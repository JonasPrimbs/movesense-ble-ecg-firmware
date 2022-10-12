from __future__ import print_function

import sys
import yaml
import re
import glob, os

from SbemCodeGenFuncs import *

# TODO: a lot of work for better maintainability

#################################################
# script debugging config for further developing
#################################################
debugEnabled = True
try:
    from PyObjTool import *
    def debugWrite(filename, string, mode="a"):
        global debugEnabled
        if not debugEnabled:
            return
        debugfile = open("/" + filename, mode)
        debugfile.write(string)
        debugfile.close()
except:
    def debugWrite(filename, string, mode=""):
        return
    def pythonObjToHumanReadable(p1, p2=0, p3=""):
        return ""

debugWrite("debug.txt", "temporary debug file...\n\n", "w")
indent = ""
for arg in sys.argv:
    indent += "  "
    debugWrite("debug.txt", indent + str(arg) + "\n")
#################################################
#################################################

oldDir = os.getcwd()


MAX_CHUNKSTORAGE_LENGTH = 112*4
MAX_NET_CHUNKSTORAGE_LENGTH = MAX_CHUNKSTORAGE_LENGTH - 2

# In case we run this code in IDLE, set some useful command line parameters
if "idlelib" in sys.modules and len(sys.argv) == 1:
    print("IDLE detected: setting command arguments")
    sys.argv.append("C:/Projects/nextgen/gcc/../movesense-gcc/package//resources/whiteboard/builtinTypes/*.yaml")
    sys.argv.append("C:/Projects/nextgen/gcc/../movesense-gcc/package//resources/whiteboard/services/*.yaml")
    sys.argv.append("C:/Projects/nextgen/gcc/../movesense-gcc/package//resources/core/*.yaml")
    sys.argv.append("C:/Projects/nextgen/gcc/../movesense-gcc/package//resources/movesense-api/*.yaml")
    sys.argv.append("C:/Projects/nextgen/gcc/../movesense-gcc/package//resources/movesense-api/meas/*.yaml")
    sys.argv.append("C:/Projects/nextgen/avocado/samples/plain_app/wbresources/*.yaml")
    sys.argv.append("C:/Projects/nextgen/gcc/../movesense-gcc/package//generated/")
    os.chdir("c:/Projects/nextgen/gcc/generated/sbem-code")

yamlFiles = []
resFileDirs = []
yamlGlobs = []
for arg in sys.argv:
    if arg.endswith(".py"):
        continue
    elif arg.endswith(".yaml"):
        yamlGlobs.append(arg)
    else:
        # Assume it's a resources.h folder
        resFileDirs.append(arg)
        print("Resource folder added: ", arg)

print("processing following yaml-files:")
for yglob in yamlGlobs:            
    for f in glob.glob(yglob):
        yamlFiles.append(f)
        print("  ", f)



# Start of program flow
#########################################
print("Processing total ", len(yamlFiles), " .yaml-files")
yamlTrees, resources, definitions = readInYamlFiles(yamlFiles)
print("Found ", len(resources), " resources and ", len(definitions), " datatype definitions.")

global dataLoggerConfig
dataLoggerConfig, fullConfig = parseDataloggerConfigFromYamlBlobs(yamlTrees)

debugWrite("datalogger-config.txt", "config:\n" + pythonObjToHumanReadable(dataLoggerConfig) + "\n", "w")
debugWrite("datalogger-config.txt", "full config:\n" + pythonObjToHumanReadable(fullConfig) + "\n", "a")

#########################################
#########################################
debugWrite("debug-yaml-tree-pre.txt", pythonObjToHumanReadable(yamlTrees) + "\n", "w")
debugWrite("debug-resources-pre.txt", pythonObjToHumanReadable(resources) + "\n", "w")
#########################################
#########################################



patternForResource = re.compile('struct ([A-Z0-9_]+)');
patternForType = re.compile('struct WB_STRUCT_PACKED ([A-Z][a-z][A-Za-z0-9]+)$');

# read resources.h and find struct definitions
cppTypes = {}
cppResources = {}
rootFound = False
lineNum = 0


# app's resources.h
resourceFiles = glob.glob('../*/resources.h')

# movesense resources.h's
for resDir in resFileDirs:
    newFiles = glob.glob(resDir + '/*/resources.h')
    resourceFiles += newFiles

#print("resourceFiles: ", resourceFiles)
resource_include_files = []
for resFile in resourceFiles:
    resFile = resFile.replace("\\", "/")
#    print("Scanning resources.h-file: ", resFile)

    separator_idx = resFile.rfind("/", 0, len(resFile) - len('/resources.h') - 1) 
    inc_file_name = resFile[separator_idx+1:]
#    print("inc_file_name: ", inc_file_name)
    resource_include_files.append(inc_file_name)

    currentResource = ""
    currentResourceName = ""
    currentType = ""
    currentTypeName = ""
    with open(resFile, 'r') as f:
        for line in iter(f):
            lineNum = lineNum +1

            # Match type structs
            m = patternForType.match(line)
            if (m):
#                print("XX type match, line ", m.groups())
                tn = m.group(1)
                currentTypeName = tn
            elif (currentTypeName != ""):
                currentType += line
                if (line.startswith("};")):
                    cppTypes[currentTypeName] = currentType
                    currentTypeName = ""
                    currentType = ""


            # match resource structs
            m = patternForResource.match(line)
            if (m):
#                print("XX Resource match: ", m.groups())
                res = m.group(1)
                if (not rootFound):
                    if (res == "ROOT"):
                        rootFound = True
                        continue

    #            print "XX resource name set"
                currentResourceName = res

            elif (currentResourceName != ""):
    #            print "XX adding line", line
                currentResource += line
                if (line.startswith("};")):
    #                print "XX end found"
                    cppResources[currentResourceName] = currentResource
                    currentResourceName = ""
                    currentResource = ""


#for k, v in cppTypes.iteritems():
#    print k, v

#for k, v in definitions.iteritems():
#    if 'properties' in v:
#        for k2,v2 in v['properties'].iteritems():
#            print str(k) + "::" + str(decapitalize(k2))

# filter out the resources that are not in cppResources
# TODO: When we get the list of all yamls in project as input, this can be skipped

newResources = {}
cppResourceNames = cppResources.keys()
for resName in resources.keys():
    if resName[0] == '/':
        cppName = cppNameFromResName(resName)
        if cppName in cppResourceNames:
            newResources[resName] = resources[resName]
            #print ("newResources: ", resName)

resources = newResources
newResources = None

# scan datatype (schema) definitions and preprocess them
simpleTypes = {}
complexTypes = {}
for k,v in sorted(definitions.iteritems()):

    # Scan properties that the datatype contains
#    print("Scan properties for: ", k)

    # store datatype into object
    v['__datatype'] = k

    # Is it a simple type, e.g. enumeration
    if 'type' in v:
        v['SBEM_KIND'] = "simple"
        simpleTypes[k] = v
        frmString = frmFromSchema(v)
#        print("simple type: ", k, "   frmString:", frmString)
        if frmString:
            v['SBEM_FRM'] = frmString
            v['SBEM_SUBIDX'] = "_X"
    else:
        # complex type, store in complexTypes for iterative processing
#        print("complex type: ", k);
        v['SBEM_KIND'] = "complex"
        complexTypes[k] = v


runAgain = True
prevCount = 65535
while(runAgain):
    notHandledTypes = {}

    for k,v in sorted(complexTypes.iteritems()):
        #print("complexTypes handling for: ", k);

        requiredProps = []
        if 'required' in v:
            requiredProps = v['required']
            #print("required: ",v['required'])

        # scan properties and fill with useful data
        #if not 'properties' in v:
            #print("Type with no properties! ", k , " : ", v)

        for propname, propvalue in v['properties'].iteritems():

            # is prop required
            propvalue['__required'] = propname in requiredProps

            if 'type' in propvalue and propvalue['type'] != 'array':
                # prop is simple property => fill SBEM_FRM
                frmString = frmFromSchema(propvalue)
                propvalue['SBEM_FRM'] = frmString
                propvalue['SBEM_KIND'] = "simple"

                #print ("simple property: ", propvalue)
            elif 'type' in propvalue and propvalue['type'] == 'array':
                # do nothing?
                #print("array property: ", propname)
                propvalue['SBEM_KIND'] = "array"

            else:
                # prop is possibly a complex object, fill datatype object
                # and handle on next round
                #print ("complex property: ", propname, ", ", propvalue)
                datatypename = propvalue['$ref'].split('/')[-1]
                propvalue['__datatype'] = datatypename

                # mark for next round if not simple and already handled
                if not 'SBEM_FRM' in definitions[datatypename]:
                    notHandledTypes[datatypename] = definitions[datatypename]
                    propvalue['SBEM_KIND'] = 'complex' # assume it's complex for now
                else:
                    #print("complex type already handled: ", datatypename, ", type: ", definitions[datatypename]);
                    propvalue['SBEM_KIND'] = definitions[datatypename]['SBEM_KIND']
                    frmString = frmFromSchema(propvalue)
                    if 'SBEM_FRM' in definitions[datatypename]:
                        propvalue['SBEM_FRM'] = definitions[datatypename]['SBEM_FRM']

    complexTypes = notHandledTypes

    runAgain = len(notHandledTypes) > 0


# Scan resources and mark their type and add found simple types to items list
print("Scanning ", len(resources), " WB-resources")

resourcesWithSimpleType = {}
resourcesWithComplexType = {}
items = []
for k,v in sorted(resources.iteritems()):
    # skip if resource does not have subscribe verb with response
#    print("resource: ", k)

    if not ('subscription' in v):
#        print("resource: ", k, " skipping, no subscription")
        continue

    if seekPathInConfig(dataLoggerConfig, "disabled-resources", k, True) is not None:
        print("LOGGING DISABLED: ", k)
        continue

    if 'subscription' in v:
        verb = v['subscription']

    # find schema from subscription definition
    if 'subscription' in v:
        schema = verb['responses']['x-notification']
        if 'schema' in schema:
            schema = schema['schema']

    frmString = frmFromSchema(schema)
    id = cppNameFromResName(k)
    #print(k, " schema:", schema, " frm: ", frmString)
    if frmString: 
        #print ("got FRM directly: ", k)
        v['SBEM_FRM'] = frmString
        v['SBEM_ID'] = id
        v['SBEM_PTH'] = resourceToSbemPath(k)
        resourcesWithSimpleType[k] = v
        item = {'id': id,
                '__property': '/',
                'SBEM_FRM': v['SBEM_FRM'],
                'SBEM_PTH': v['SBEM_PTH']}
        items.append(item)
        v['SBEM_ITEM'] = item

    # didn't get FRM directly
    # study datatypes and try to get from there
    elif '$ref' in schema:
#        print ("'$ref' in schema:: ", k)
        datatypename = schema['$ref'].split('/')[-1]
#        print ("datatypename: ", datatypename)

        if datatypename in definitions:
            datatype = definitions[datatypename]
            v['__datatype'] = datatypename

            if 'SBEM_FRM' in datatype:
                # simple datatype, add to gen resources and get FRM from datatype
                v['SBEM_KIND'] = "simple"
                v['SBEM_FRM'] = datatype['SBEM_FRM']
                v['SBEM_ID'] = id
                v['SBEM_PTH'] = resourceToSbemPath(k)

                resourcesWithSimpleType[k] = v

                item = {'id': id,
                        '__property': '/',
                        'SBEM_FRM': v['SBEM_FRM'],
                        'SBEM_PTH': v['SBEM_PTH']}
                items.append(item)
                v['SBEM_ITEM'] = item

                #print("   - was SIMPLE")

            else:
                # complex datatypes are generated into groups in the next phase
                #print ("Complex datatype ", k)
                v['SBEM_KIND'] = "complex"
                resourcesWithComplexType[k] = v                        

        else:
            print ("Unknown Datatype, skipping... ", k)


# Creating groups for complex datatypes
print("Creating SBEM-Groups")
groups = []
for k,v in sorted(resourcesWithComplexType.iteritems()):

    resource = v
    if not '__datatype' in resource:
        #print("Skipping resource ",k," for lack of __datatype key.")
        continue

    #print("resource With Complex Type ",k)

    datatype = definitions[resource['__datatype']]
    #print(" - resource datatype: ", datatype)

    resourceItems = []
    resourceGroups = []
    resource['SBEM_PTH'] = k
    resourceParentGroup = generateGroupsForResAndDatatype(definitions, resource,
                                    "/", datatype, resourceGroups, resourceItems)

    if not resourceParentGroup:
        #print("resourceParentGroup was None, won't generate group")
        continue

    #print(" - groupItems: ", resourceItems)
    #print(" - resourceGroups: ", resourceGroups)
    #print(" - resourceParentGroup: ", resourceParentGroup)

    items.extend(resourceItems)
    groups.extend(resourceGroups)
    groups.append(resourceParentGroup)

    # Since SBEM only supports flat groups, flatten it
    resourceParentGroup['SBEM_GRP_ITEMS'] = resourceItems

    # and store the group to resource
    v['SBEM_GRP'] = resourceParentGroup


# Generate the C++ code

# make sure there are no duplicates
handled_items = set()
u_items = []
for item in items:
    if item['id'] in handled_items:
        continue
    handled_items.add(item['id'])
    u_items.append(item)

items = u_items

print("Generating C++ code. ", len(groups), " groups, ", len(items), " items")

with open("sbem_definitions.h", 'wb') as f_h:
    with open("sbem_definitions.cpp", 'wb') as f_cpp:
        # write top of h.file
        print('#pragma once', file=f_h)
        print('#include "sbem_types.h"', file=f_h)
        print('#include "common/core/dbgassert.h"', file=f_h)
        print("", file=f_h)

        # write top of cpp.file
        print('#include "whiteboard/Value.h"', file=f_cpp)
        for inc_file in resource_include_files:
            print('#include "'+inc_file+'"', file=f_cpp)
        #print('#include "wb-resources/resources.h"', file=f_cpp)
        print('#include "sbem_definitions.h"', file=f_cpp)


        # output the group definitions to cpp file
        # Groups are defined with a macro "CREATE_GROUP" that takes in the Item ID's
        # sample:
        #    CREATE_GROUP(MAGNETOMETERGroup,
        #        static_cast<uint16>(MAGNETOMETER_X),
        #        static_cast<uint16>(MAGNETOMETER_Y),
        #        static_cast<uint16>(MAGNETOMETER_Z));

        groupIdEntries = []
        arrayGroupStartItems = {}
        rootGroupStartItems = {}
        for group in groups:
            # Only resource groups are relevant, those have 'subgroups'
            if not 'subgroups' in group:
                continue

            elements = [1] # elements of 1 == no array

            subgroupsContainArray = False
            for subgrp in group['subgroups']:
                if '__isArray' in subgrp and subgrp['__isArray']:
                    subgroupsContainArray = True
                    arrayPath = "/" + subgrp["id"].replace("_","/")

                    elements = seekArrayElementsConfig(dataLoggerConfig, "resources", arrayPath, True)
                    break

            for i in elements:
                groupId = group['id']
                if subgroupsContainArray:
                    group['__isArrayGroup'] = True
                    groupId = groupId + str(i)

                print("DECLARE_GROUP(" + groupId + ")", file=f_h)

                f_cpp.write("\nCREATE_GROUP(" + groupId)

                # append group declaration to list to be written later to h-file
                groupIdEntries.append("    " + groupId + ",\n")

                firstItem = True
                for item in group['subgroups']:
                    loopCounter = 1
                    arrayBeginTag = False
                    if '__isArray' in item and item['__isArray']:
                        loopCounter = i
                        arrayBeginTag = True
                        arrayBeginItemId = 'ARRAY_BEGIN'
                        print(",", file=f_cpp)
                        f_cpp.write("    static_cast<uint16_t>(" + arrayBeginItemId + ")")

                    for loopIdx in range(loopCounter):
                        print(",", file=f_cpp) # end of line and comma
                        f_cpp.write("    static_cast<uint16_t>(" + item['id'] + ")")
                        if firstItem and '__isArray' in group and group['__isArray']:
                            arrayGroupStartItems[item['id']] = group
                            #print("array group found: ", group['id'])
                        elif firstItem and group['__property'] == '/':
                            #print("item: ", item['id'], " is first item of group: ", groupId)
                            rootGroupStartItems[item['id']] = group

                    # add array end tag
                    if arrayBeginTag:
                        print(",", file=f_cpp)
                        f_cpp.write("    static_cast<uint16_t>(ARRAY_END)")

                    firstItem = False

                print(");", file=f_cpp)

        # Now generate SBEM DescriptionItems array:
        #  declaration in h, implementation in cpp
        count, descriptorItemIds, itemIndexes = generateDescriptorArray(f_h, f_cpp, items, arrayGroupStartItems, rootGroupStartItems)

#        print('\nextern const DescriptorItem_t s_possibleSbemItems[' +str(count) + '];', file=f_h)

        # Generate descriptorItem list enum in h-file
        # start with stock SBEM id's
        print('\nenum SbemValueIds_e {', file=f_h)
        print('COMMON_DESCRIPTOR = CommonValueIds_e::DESCRIPTOR,', file=f_h)
        print('COMMON_SOURCE = CommonValueIds_e::SOURCE,', file=f_h)
        print('COMMON_TIMEISO8601 = CommonValueIds_e::TIMEISO8601,', file=f_h)

        for itemId in descriptorItemIds:
            print("    " + itemId + ",", file=f_h)

        # ARRAY_BEGIN & END tags
        print("    ARRAY_BEGIN,", file=f_h)
        print("    ARRAY_END,", file=f_h)

        # end ID enum
        print("    SbemValueIds_COUNT,", file=f_h)
        print("};", file=f_h)


        # output the group ids enum in h-file
        print('\nenum SbemGroupIds_e {', file=f_h)
        print('    BEFORE_GROUPS_ID = SbemValueIds_COUNT-1,', file=f_h)

        f_h.write("".join(groupIdEntries))

        # end SBEM groups id array
        print("    SbemAllIds_COUNT", file=f_h)
        print("};", file=f_h)


        # Generate groups array. declaration in h, definition in cpp
        count = 0
        print("\nconst DescriptorGroup_t s_possibleSbemGroups[] = {", file=f_cpp)        
        for group in groups:
            # Only resource groups are relevant, those have 'subgroups'
            if not 'subgroups' in group:
                continue

            #groupPath = "__dummy" # init with dummy path to get no match and default array config
            groupPath = "/" + group['id'].replace("_","/")

            if "SBEM_GRP_ITEMS" in group:
                for listItem in group["SBEM_GRP_ITEMS"]:
                    if "SBEM_PTH" in listItem:
                        groupPath = "/" + listItem["SBEM_PTH"].replace(".","/")
                        # just fetch the first path declaration in the list
                        break
            if "SBEM_PTH" in group:
                groupPath = group["SBEM_PTH"]

            elements = [1]
            subgroupsContainArray = False
            if '__isArrayGroup' in group and group['__isArrayGroup']:
                subgroupsContainArray = True

                elements = seekArrayElementsConfig(dataLoggerConfig, "resources", groupPath, True)

            for i in elements:
                groupId = group['id']

                if subgroupsContainArray:
                    groupId = groupId + str(i)

                count += 1

                print("    " + groupId + "_Group,", file=f_cpp)

        print("    {ID_DESCRIPTOR_ARRAY_END_MARKER, 0, NULL}", file=f_cpp)
        print("};", file=f_cpp)

        #print("\nextern const DescriptorGroup_t s_possibleSbemGroups[" + str(count) + "];", file=f_h)        

        resourcesThatNeedSbemMethods = {}

        # Scan resources and choose those that need methods generated for them
        for k,v in sorted(resources.iteritems()):
            writeMethodCode = ""
            itemsToGenerate = []
            if 'SBEM_ITEM' in v:
                itemsToGenerate = [v['SBEM_ITEM']]
            elif 'SBEM_GRP' in v:
                #print("v['SBEM_GRP']: ", v['SBEM_GRP'])
                itemsToGenerate = [v['SBEM_GRP']]
            else:
                continue

            # handle each item entry in resource value, except those we can't handle            
            takeThis = not hasUnsupportedItems(itemsToGenerate)

            if takeThis:
                # Pre-process the items & groups so that they are easy to convert to code
                preprocessItemsToGenerate(itemsToGenerate)

                # store the processes items to the resource
                resourcesThatNeedSbemMethods[k] = v
                resourcesThatNeedSbemMethods[k]['__itemsToGenerate'] = itemsToGenerate
                #print("takeThis: ", k, " itemsToGenerate: ", itemsToGenerate)
            else:
                print("skipping unsupported resource: ", k)

        # Generate functions to write wbObject as SBEM into buffer
        for k,v in sorted(resourcesThatNeedSbemMethods.iteritems()):
            itemsToDo = v['__itemsToGenerate']

            # reset loop index numbering
            setLoopNumber(1)

            cppName = cppNameFromResName(k)
            writeMethodCode = '\nsize_t writeToSbemBuffer_' + cppName + '(void *buffer, size_t bufferLen, size_t startOffset, const WB_RES::LOCAL::' + cppName + '::EVENT::NotificationType &data)'
            writeMethodCode += ' {\n'
            writeMethodCode += '    size_t bytesRead = 0;\n'
            writeMethodCode += '    size_t bytesWritten = 0;\n'
            writeMethodCode += '    uint8_t *byteBuf = static_cast<uint8_t*>(buffer);\n'

            # handle each item entry in resource value
            codeOk = True
            writeCodeArr = []
            for item in itemsToDo:
                codeOk = codeOk and generateWriteMethodCodeForItem(item, writeCodeArr)

            if codeOk:
                writeMethodCode += "".join(writeCodeArr).replace('@@', 'data')
            else:
                writeMethodCode += '    // UNIMPLEMENTED FEATURE in codegen, so we assert;\n'
                writeMethodCode += '    ASSERT(0);\n'

            writeMethodCode += '    return bytesWritten;\n'
            writeMethodCode += '}\n\n'

            print(writeMethodCode, file=f_cpp)

            print('\nsize_t writeToSbemBuffer_' +cppNameFromResName(k) + '(void *buffer, size_t bufferLen, size_t startOffset, const WB_RES::LOCAL::' + cppNameFromResName(k) + '::EVENT::NotificationType &data);', file=f_h)


        # Generate function to calculate sbem packet length for given resource & data
        for k,v in sorted(resourcesThatNeedSbemMethods.iteritems()):
            getLengthMethodCode = ""
            itemsToDo = v['__itemsToGenerate']

            getLengthMethodCode += '\nsize_t getSbemLength_' +cppNameFromResName(k) + '(const WB_RES::LOCAL::' + cppNameFromResName(k) + '::EVENT::NotificationType &data)\n'
            getLengthMethodCode += '{\n'

            # handle each item entry in resource value
            codeOk = True
            lengthCodeArr = []
            for item in itemsToDo:
                codeOk = codeOk and generateLengthMethodCodeForItem(item, writeCodeArr, lengthCodeArr)

            if not codeOk:
                getLengthMethodCode += '    // UNIMPLEMENTED FEATURE in codegen, so we assert;\n'
                getLengthMethodCode += '    ASSERT(0);\n'
            else:
                getLengthMethodCode += '    return (0' + "".join(lengthCodeArr).replace('@@', 'data') + ');\n'

            getLengthMethodCode += '}\n\n'

            # write code to cpp file
            print(getLengthMethodCode, file=f_cpp)

            # and declaration to .h file
            print('\nsize_t getSbemLength_' +cppNameFromResName(k) + '(const WB_RES::LOCAL::' + cppNameFromResName(k) + '::EVENT::NotificationType &data);', file=f_h)

        # Generate generic getSbemLengthConst method (for resources with fixed size data)
        getLengthMethodCode = '\nint16_t getSbemLengthConst(whiteboard::LocalResourceId localResId)\n'
        getLengthMethodCode += '{\n'
        getLengthMethodCode += '    switch(localResId)\n'
        getLengthMethodCode += '    {\n'

        for k,v in sorted(resourcesThatNeedSbemMethods.iteritems()):
            itemsToDo = v['__itemsToGenerate']

            if not hasVariableLengthItems(itemsToDo):
                getLengthMethodCode += '    case WB_RES::LOCAL::' + cppNameFromResName(k) + '::LID:\n'
                sizeExpr = str(getVariableLengthItems(itemsToDo))
                # print(sizeExpr)
                getLengthMethodCode += '        return ('+sizeExpr+');\n'

        getLengthMethodCode += '    default:\n'
        getLengthMethodCode += '        return -1;\n'
        getLengthMethodCode += '    }\n'
        getLengthMethodCode += '}\n'

        print(getLengthMethodCode, file=f_cpp)


        # Generate generic getSbemLength method
        getLengthMethodCode = '\nsize_t getSbemLength(whiteboard::LocalResourceId localResId, const whiteboard::Value &data)\n'
        getLengthMethodCode += '{\n'
        getLengthMethodCode += '    switch(localResId)\n'
        getLengthMethodCode += '    {\n'

        for k,v in sorted(resourcesThatNeedSbemMethods.iteritems()):
            getLengthMethodCode += '    case WB_RES::LOCAL::' + cppNameFromResName(k) + '::LID:\n'
            getLengthMethodCode += '        return getSbemLength_' +cppNameFromResName(k) + '(data.convertTo<WB_RES::LOCAL::' + cppNameFromResName(k) + '::EVENT::NotificationType>());\n'


        getLengthMethodCode += '    }\n'
        getLengthMethodCode += '    DEBUGLOG("getSbemLength error: Unknown localResourceId: %u", localResId);\n'        

        getLengthMethodCode += '    return 0;\n'
        getLengthMethodCode += '}\n'

        print(getLengthMethodCode, file=f_cpp)


        # Generate generic write bwobject method
        writeMethodCode = '\nsize_t writeToSbemBuffer(void *buffer, size_t bufferLen, size_t startOffset, whiteboard::LocalResourceId localResId, const whiteboard::Value &data)\n'
        writeMethodCode += '{\n'
        writeMethodCode += '    switch(localResId)\n'
        writeMethodCode += '    {\n'

        for k,v in sorted(resourcesThatNeedSbemMethods.iteritems()):
            writeMethodCode += '    case WB_RES::LOCAL::' + cppNameFromResName(k) + '::LID:\n'
            writeMethodCode += '        return writeToSbemBuffer_' +cppNameFromResName(k) + '(buffer, bufferLen, startOffset, data.convertTo<WB_RES::LOCAL::' + cppNameFromResName(k) + '::EVENT::NotificationType>());\n'


        writeMethodCode += '    }\n'        
        writeMethodCode += '    DEBUGLOG("writeToSbemBuffer error: Unknown localResourceId: %u", localResId);\n'        

        writeMethodCode += '    return 0;\n'
        writeMethodCode += '}\n'

        print(writeMethodCode, file=f_cpp)


        # Generate resourceId => SbemItem mapping table (single items)
        tableString = "\nconst SbemResID2ItemIdMapping s_sbemResID2ItemIdMap[] = {\r\n"
        count = 0
        for k,v in sorted(resourcesThatNeedSbemMethods.iteritems()):
            if 'SBEM_GRP' in v:
                continue

            if 'SBEM_ID' in v:
                count += 1
                tableString += "    {WB_RES::LOCAL::" + cppNameFromResName(k) + "::LID, &s_possibleSbemItems[" + str(itemIndexes[v['SBEM_ID']]) + "] },\r\n"

        tableString += "    ITEM_MAPPING_END_MARKER\r\n"
        tableString += "};"

        if (count > 0):
            print(tableString, file=f_cpp)
            #print("\nextern const SbemResID2ItemIdMapping s_sbemResID2ItemIdMap[" + str(count) + "];", file=f_h)
        else:
            print("const SbemResID2ItemIdMapping s_sbemResID2ItemIdMap[] = { 0, {0, 0} };", file=f_cpp) # no data, inject dummy table
            #print("\nextern const SbemResID2ItemIdMapping s_sbemResID2ItemIdMap[1];", file=f_h)


        # Generate resourceId => SbemGrp mapping table
        print("\nconst SbemResID2GrpIdMapping s_sbemResID2GrpIdMap[] = {", file=f_cpp)
        count = 0
        for k,v in sorted(resourcesThatNeedSbemMethods.iteritems()):
            if not 'SBEM_GRP' in v:
                continue

            for item in v['SBEM_GRP']['SBEM_GRP_ITEMS']:
                if not item['SBEM_FRM']:
                    print("SKIPPING valueExpression! SBEM_FRM was None for ", item['SBEM_PTH'])
                    continue

            itemsToDo = v['__itemsToGenerate']
            lengthCodeArr = []
            for item in itemsToDo:
                generateLengthMethodCodeForItem(item, writeCodeArr, lengthCodeArr)

            subgroupsContainArray = False
            if hasVariableLengthItems(itemsToDo):
                subgroupsContainArray = True

            lengthCode = "".join(lengthCodeArr)

            # replace all length expressions with the @#@ tag        
            sizePartRegex = re.compile(r'@@[\.\w]+.size\(\)')
            lengthCode = sizePartRegex.sub("@#@", lengthCode)

            elements = [1] # init with no array (e.g. one element)
            if subgroupsContainArray:
                elements = seekArrayElementsConfig(dataLoggerConfig, "resources", v['SBEM_PTH'])

            # create version or all groups for arrays, either custom array lengths or 1-8
            for i in elements:
                lengthInBytesExpr = lengthCode.replace('@#@', " (" + str(i) + ") ")
                lengthInBytes = eval(lengthInBytesExpr)

                if lengthInBytes > MAX_NET_CHUNKSTORAGE_LENGTH:
                    print("Skipping",v['SBEM_GRP']['id'],"size()==" + str(i),"because data too long.")
                    continue
                
#                print("lengthInBytesExpr:", lengthInBytesExpr, " = ", lengthInBytes)
                count += 1
                if not subgroupsContainArray:
                    groupId = v['SBEM_GRP']['id'] + "_Group"
                else:
                    groupId = v['SBEM_GRP']['id'] + str(i) + "_Group"
                print("    { WB_RES::LOCAL::" + cppNameFromResName(k) + "::LID, " + lengthInBytesExpr + ", &" + groupId + " },", file=f_cpp)
        print("    GROUP_MAPPING_END_MARKER", file=f_cpp)
        print("};", file=f_cpp)

        #print("extern const SbemResID2GrpIdMapping s_sbemResID2GrpIdMap[" + str(count) + "];", file=f_h)


        # Generate resourceId => sbemId mapper function

        # print("\nuint16_t getSbemDescriptorIdFromResource(whiteboard::LocalResourceId localResourceId, size_t dataLength)", file=f_cpp)
        # print("{", file=f_cpp)
        # print("    for (size_t i=0; i<ELEMENTS(s_sbemResID2ItemIdMap); i++)", file=f_cpp)
        # print("    {", file=f_cpp)
        # print("        if (s_sbemResID2ItemIdMap[i].wbResId == localResourceId)", file=f_cpp)
        # print("        {", file=f_cpp)
        # print("            return s_sbemResID2ItemIdMap[i].sbemItem.id;", file=f_cpp)
        # print("        }", file=f_cpp)
        # print("    }\n", file=f_cpp)
        # print("    for (size_t i=0; i<ELEMENTS(s_sbemResID2GrpIdMap); i++)", file=f_cpp)
        # print("    {", file=f_cpp)
        # print("        if (s_sbemResID2GrpIdMap[i].wbResId == localResourceId && s_sbemResID2GrpIdMap[i].sbemBytes == dataLength)", file=f_cpp)
        # print("        {", file=f_cpp)
        # print("            return s_sbemResID2GrpIdMap[i].sbemGrp.id;", file=f_cpp)
        # print("        }", file=f_cpp)
        # print("    }", file=f_cpp)
        # print("    return 0;", file=f_cpp)
        # print("}", file=f_cpp)

        # Generate item & group accessors

        print("\nuint16_t getSbemItemsCount()", file=f_cpp)
        print("{", file=f_cpp)
        print("    return SbemValueIds_COUNT;", file=f_cpp)
        print("}", file=f_cpp)

        print("\nuint16_t getSbemGroupsCount()", file=f_cpp)
        print("{", file=f_cpp)
        print("    return SbemAllIds_COUNT - SbemValueIds_COUNT;", file=f_cpp)
        print("}", file=f_cpp)

        print("\nuint16_t getFirstSbemGroupId()", file=f_cpp)
        print("{", file=f_cpp)
        print("    return BEFORE_GROUPS_ID + 1;", file=f_cpp)
        print("}", file=f_cpp)


if "idlelib" in sys.modules and len(sys.argv) == 1:
    os.chdir(oldDir)

#########################################
#########################################

debugWrite("debug-yaml-tree-post.txt", pythonObjToHumanReadable(yamlTrees) + "\n", "w")
debugWrite("debug-resources-post.txt", pythonObjToHumanReadable(resources) + "\n", "w")

