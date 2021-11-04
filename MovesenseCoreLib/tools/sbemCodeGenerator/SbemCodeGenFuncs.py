
from __future__ import print_function

import sys
import yaml
import re

import glob, os

try:
    from PyObjTool import *
except:
    def pythonObjToHumanReadable(dummy, dummy2=0, dummy3=""):
        return dummy3

decapitalize = lambda s: s[:1].lower() + s[1:] if s else ''
endNumRegex = re.compile(r'.*[^\d](\d+)$')

# Function definitions
def readInYamlFiles(yamlFiles):
    yamlTrees = []
    resources = {}
    definitions = {}

    for f in yamlFiles:
        stream = file(f, 'r')
        obj = yaml.load(stream)
        obj["fromFile"] = f

        yamlTrees.append(obj)

        if 'paths' in obj:
            for k, v in sorted(obj['paths'].iteritems()):
                if k.endswith('/Subscription'):
                    k = k[:-len('/Subscription')]
                    if k not in resources:
                        resources[k] = {}
                        #print("merged subscription to parent path: ",k)
                    resources[k]['subscription'] = v['post']
                else:
                    resources[k] = v

        if ('definitions' in obj):
            for k, v in obj['definitions'].iteritems():
                definitions[k] = v
                #print("yaml datatype: ", k, " ", v)

        stream.close()

    return yamlTrees, resources, definitions

def parseDataloggerConfigFromYamlBlobs(yamlBlobs):
    config = {"resources": dict(), "disabled-resources": list()}
    full = {"resources": dict()}

    for fileYamlBlob in yamlBlobs:
        yamlFileItems = fileYamlBlob.items()
        for dictName,dictObj in yamlFileItems:
            if dictName == "datalogger":
                if "resources" in dictObj:
                    config["resources"].update(dictObj["resources"])
                    for path in dictObj["resources"]:
                        pathDict = dictObj["resources"][path]

                        # handle datalogger::resources::path::enabled token in yaml
                        if "enabled" in pathDict and pathDict["enabled"] is False:
                            # resource path was explicitly disabled in yaml
                            config["disabled-resources"].append(path)
                            continue

                        # handle datalogger::resources::path::array-lengths token in yaml
                        if "array-lengths" in pathDict:
                            if path not in config["resources"]:
                                config["resources"][path] = dict()

                            if isinstance(pathDict["array-lengths"], int):
                                config["resources"][path]["array-lengths"] = [pathDict["array-lengths"]]
                            elif isinstance(pathDict["array-lengths"], list):
                                config["resources"][path]["array-lengths"] = pathDict["array-lengths"]
                            elif isinstance(pathDict["array-lengths"], str):
                                config["resources"][path]["array-lengths"] = [int(num) for num in pathDict["array-lengths"].split(',')]

                        # handle datalogger::resources::path::conversion token in yaml
                        if "conversion" in pathDict:
                            config["resources"][path]["conversion"] = pathDict["conversion"]
                            #if "parameters" in pathDict["conversion"]:
                            #    params = [str(p) for p in pathDict["conversion"]["parameters"].replace(" ","").split(',')]
                            #    config["resources"][path]["conversion"]["parameters"] = params

                    full["resources"].update(dictObj["resources"])

    return config, full #{"config": config, "full-input-config": full}

# Check if given resource path has any configuration in the given config dictionary.
# Returns None if no configuration found, or the first list string or name (key) of the dict entry found.
# Forcing partial match is needed when paths deduced from CPP files contain the structure name at the end.
def seekPathInConfig(config, dictName, pathToMatch, forcePartialMatch=False):
    if pathToMatch[0] == '/':
        pathToMatch = pathToMatch[1:]
    pathToMatch = pathToMatch.replace(".", "/").replace("+","/")

    for rscPath in config[dictName]:
        tmpPath = rscPath.replace("{","").replace("}","")[1:] # remove path parameter braces, move this to config construction?
        # pathToMatch will also have brackets if read from yaml, if read from cpp/h then not

        # full path match, prefer this over partial match
        if tmpPath == pathToMatch or rscPath == pathToMatch:
            #print("MATCH 1 " + rscPath + " - " + pathToMatch)
            return rscPath

        # partial match from beginning
        if tmpPath.endswith(".*") or forcePartialMatch:
            # first try the one with brackets removed
            if re.search(tmpPath[:-2], pathToMatch, re.IGNORECASE):
                #print("MATCH 2 " + rscPath + " - " + pathToMatch)
                return rscPath
            # second try the one that may have brackets
            if re.search(rscPath[:-2], pathToMatch, re.IGNORECASE):
                #print("MATCH 3 " + rscPath + " - " + pathToMatch)
                return rscPath

    return None

# Search for array element config for the given path in the given config under the dictionary name
# return a list of enum values e.g. [2,4,8], default to [1,2,3,4,5,6,7,8] if nothing found
def seekArrayElementsConfig(config, dictName, rscPath, isCppType=False):
    elements = range(1, 8+1) # default to 1 to 8 range for arrays

    # check if path has a match in resources (dict type), result will be None or the key (mathing path string)
    resultString = seekPathInConfig(config, dictName, rscPath, isCppType)

    # resultString may be a match for multiple resources, e.g. "/Meas/IMU.*"
    if resultString is not None:
        # path matching the config found, check if array config available
        if "array-lengths" in config[dictName][resultString]:
            elements = config[dictName][resultString]["array-lengths"]

    #print("[][][] -- " + rscPath + " " + str(elements))

    return elements

# Search for type conversion config (e.g. from float to int16) in the given config
# returns the conversion config matching the given resource path, or None if nothing found
# return example:
#    DICT
#    {
#        "x":
#          "to": "int16",
#          "encoding": (value - 15*16.1f) / 16.1f
#          "decoding": (value + 15) * 16.1f
#        "y":
#          "to": "int16",
#          "encoding": (value - 15*16.1f) / 16.1f
#          "decoding": (value + 15) * 16.1f
#    }
def seekConversionConfig(config, dictName, rscPath, isCppType=False):
    # check if path has a match in resources (dict type), result will be None or the key (mathing path string)
    resultString = seekPathInConfig(config, dictName, rscPath, isCppType)
    conversion = None

    # resultString may be a match for multiple resources, e.g. "/Meas/IMU.*"
    if resultString is not None:
        #print("... " + str(config[dictName][resultString]))
        # path matching the config found, check if array config available
        if "conversion" in config[dictName][resultString]:
            #if "parameters" in config[dictName][resultString]["conversion"]:
            conversion = config[dictName][resultString]["conversion"]

    #print("CONV " + rscPath + " (match: " + str(resultString) + "): " + str(conversion))
    return conversion

def cppNameFromResName(resName):
    return resName[1:].replace('/','_').replace('{','').replace('}','').upper()

def resourceToSbemPath(resourcePath):
    return resourcePath[1:].replace('/','.').replace('{','').replace('}','')

def frmFromResponse(response):
    return frmFromSchema(response['schema'])

def frmFromSchema(schema):
    if 'type' in schema:
        typedef = schema['type']
        if typedef == 'number' and schema['format']=='float':
            return 'float32'
        if typedef == 'number' and schema['format']=='double':
            return 'float64'
        elif typedef == 'integer':
            return schema['format']
        elif typedef == 'string':
            return 'utf8'
        elif typedef == 'boolean':
            return 'bool'
        elif typedef == 'objec': 
            return None
        elif typedef == 'object': 
            return None
        elif typedef == 'array': # XXXX TODO!5
            raise Exception('type == array should not be handled here!')

        print("unknown typedef. schema: ", schema)
        raise Exception('Unsupported typedef: ' + str(typedef))

def sbemNullValueFromFormat(format):
    if format == 'float32':
        return '-99999999999.0';
    if format == 'float64':
        return '-99999999999.0';
    elif format == 'int64':
        return '4294967295';
    elif format == 'int32':
        return '2147483647';
    elif format == 'int16':
        return '32767';
    elif format == 'int8':
        return '127';
    elif format == 'uint32':
        return '4294967295';
    elif format == 'uint64':
        return '4294967295';
    elif format == 'uint16':
        return '65535';
    elif format == 'uint8':
        return '255';
    else:
        raise Exception("UNSUPPORTED SBEM NULL TYPE: " + str(format))

def sbemNullValueFromItem(item):
    return sbemNullValueFromFormat(item['SBEM_FRM'])

def cppNullValueFromItem(item):
    frm = item['SBEM_FRM']
    if frm == 'float32':
        return '-99999999999.0f';
    return sbemNullValueFromItem(item)

def sizeExprFromItem(item):
    frm = item['SBEM_FRM']

    # swap format from conversion[param]["to"] spec if available
    fullPath = item["SBEM_PTH"].replace("+",".")
    paramName = fullPath.split(".")[-1]
    from sbemCodeGen import dataLoggerConfig
    conversionConf = seekConversionConfig(dataLoggerConfig, "resources", item['SBEM_PTH'], True)
    if conversionConf is not None and paramName in conversionConf and "to" in conversionConf[paramName]:
        frm = conversionConf[paramName]["to"]
    
    if frm.startswith('float') or frm.startswith('int') or frm.startswith('uint'):
        m = endNumRegex.match(frm)
        sizeExpression = int(m.groups(1)[0])/8

    elif frm == 'utf8':
        print("frm == 'utf8' item: " , item)
        propertyName = "XXXXX"
        sizeExpression = 'strlen(data.' + propertyName + ')'
        return None
    
    elif frm == 'bool':
        sizeExpression = 1

    else:
       raise Exception("sizeExprFromItem: UNSUPPORTED SBEM NULL TYPE: ", str(frm))

    return sizeExpression


def getItemPropChain(item):
    # collect the chain of parent groups 
    propchain = [item]
    grp = item
    while '__parentGrp' in grp:
        grp = grp['__parentGrp']
        propchain.append(grp)

    propchain.reverse()

    return propchain

def itemIsOptional(item):
    #print("#### itemIsOptional: ", item['SBEM_PTH'])
    frm = item['SBEM_FRM']
    if frm == 'utf8':
        return False
    if frm == 'bool':
        return False

    propchain = getItemPropChain(item)
    for prop in propchain:
        if not '__required' in prop or not prop['__required']:
            return True

    return False


loopNumber = 1

def setLoopNumber(value):
    global loopNumber
    loopNumber = value

def getLoopNumber():
    global loopNumber
    return loopNumber

def valueExprFromItem(item, includeIndexing = True):
    global loopNumber
#    if '__parentGrp' in item:
#        print(" parent prop: ", item['__parentGrp']['__property'], ", required=",item['__parentGrp']['__required'])

    propchain = getItemPropChain(item)

#    print("valueExprFromItem for item: ", item['__property'])
    # find out the possible wb::Optional's
    hasValueExpr = ""
    propertyExpr = ""
    lastUsedLoopnumber = loopNumber-1
    for prop in propchain:
        if len(propertyExpr)>0:
            propertyExpr += "."

        propterm = prop['__property']
        if propterm.endswith('/'):
            propterm = propterm[:-1]

        # Take last piece of path
        propterm = propterm.split('/')[-1]


#        print("- property: ", propterm) #, " : ", prop)
        propertyExpr += decapitalize(propterm)

        isArrayProperty = '__isArray' in prop and prop['__isArray']

        # Index here for each loop of array
        if isArrayProperty and includeIndexing:
            # write for loop for traversing array
            currentLoopVariable = "loopidx_" + str(lastUsedLoopnumber)
            lastUsedLoopnumber = lastUsedLoopnumber -1

            propertyExpr += '[' + currentLoopVariable + ']'

        if '__required' in prop and not prop['__required']:
            if len(hasValueExpr) > 0:
                hasValueExpr += " && " 
            hvexpr = "@@." + propertyExpr + ".hasValue()"
            hasValueExpr += "(" + hvexpr + ")"
            propertyExpr += ".getValue()"

    # make sure that this really is a nullable type. if type is not found, we cant make it anywaus

    if len(hasValueExpr) > 0:
        return "(" + hasValueExpr + ") ? @@." + propertyExpr + " : " + cppNullValueFromItem(prop)

    if (len(propertyExpr)>0):
        propertyExpr = "." + propertyExpr
    return "@@" + propertyExpr

def hasVariableLengthItems(itemsToGenerate):
    for item in itemsToGenerate:
        # if this item has subgroups or items, clean them as well
        isArray = '__isArray' in item and item['__isArray']
        if isArray:
            return True
        if 'SBEM_FRM' in item and item['SBEM_FRM'] == 'utf8':
            return True
        elif 'subgroups' in item:
            if hasVariableLengthItems(item['subgroups']):
                return True
    return False

def getVariableLengthItems(itemsToGenerate):
    size = 0
    for item in itemsToGenerate:
        if 'subgroups' in item:
            size += getVariableLengthItems(item['subgroups'])
        else:
            size += sizeExprFromItem(item)
    return size

def hasUnsupportedItems(itemsToGenerate):
    for item in itemsToGenerate:
        # if this item has subgroups or items, clean them as well
        #isArray = '__isArray' in item and item['__isArray']
        if 'SBEM_FRM' in item and item['SBEM_FRM'] == 'utf8':
            return True
        elif 'subgroups' in item:
            if hasUnsupportedItems(item['subgroups']):
                return True
        elif not 'SBEM_FRM' in item:
            return True
        
    return False

def preprocessItemsToGenerate(itemsToGenerate):
    for itemOrGroup in itemsToGenerate:
        # remove __parentGrp, makes easier to read for debugging
        #itemOrGroup.pop('__parentGrp', None)
        # remove SBEM_GRP_ITEMS
        #itemOrGroup.pop('SBEM_GRP_ITEMS', None)

        # if this item has subgroups or items, clean them as well
        if 'subgroups' in itemOrGroup:
            preprocessItemsToGenerate(itemOrGroup['subgroups'])

def generateLengthMethodCodeForItem(item, writeCodeArr, lengthCodeArr):
    # Make sure that if item is marked as unsupported, we don't create code for it
    if writeCodeArr == None:
        return False
    if (('__INSERT_ASSERT' in item) and item['__INSERT_ASSERT']):
        del writeCodeArr[:] # clear all code and return false
        return False

    lengthMethodCode = ""

    isArray = '__isArray' in item and item['__isArray']
    methodCode_ArrayEnd = None
    methodCode_ArrayBegin = None
    if isArray:
        arrayLenExpr = valueExprFromItem(item, False)+'.size()'

        # write for loop for traversing array
        methodCode_ArrayBegin = arrayLenExpr + ' * (0 '
        methodCode_ArrayEnd = ' ) '
        lengthCodeArr.append(' + ')
        lengthCodeArr.append(methodCode_ArrayBegin)


    if 'SBEM_FRM' in item:
        # length code for individual item (e.g. int64)
        sizeExpr = sizeExprFromItem(item)
        if sizeExpr:
            lengthCodeArr.append(' + ')
            lengthCodeArr.append(str(sizeExpr))

    elif 'subgroups' in item and item['subgroups']:
        # write for loop for traversing array
        for grp in item['subgroups']:
            #print("handling length code for subgroup: ", grp)
            generateLengthMethodCodeForItem(grp, writeCodeArr, lengthCodeArr)

    else:
        print("generateLengthMethodCodeForItem NOT IMPLEMENTED: ", item)

    if methodCode_ArrayEnd:
        lengthCodeArr.append(methodCode_ArrayEnd)

    return True

def generateDescriptorArray(hpp_file, cpp_file, items, arrayGroupStartItems, rootGroupStartItems):
    # generate DescriptorItem_t array start
    print('\nconst DescriptorItem_t s_possibleSbemItems[] = {', file=cpp_file)
    print("\n", file=hpp_file)
    count = 0
    itemIndexes = {}
    alreadyPlussedResources = set()
    descriptorItemIds = []
    for item in items:
        if (not 'SBEM_FRM' in item) or not item['SBEM_FRM']:
            #print("Skipping item without SBEM_FRM! item:", item)
            continue

        isNillable = itemIsOptional(item)
        isFirstInArray = \
            bool(item['id'] in arrayGroupStartItems and arrayGroupStartItems[item['id']])
        isFirstInRootGroup = \
            bool(item['id'] in rootGroupStartItems and rootGroupStartItems[item['id']])
        itemIndexes[item['id']] = count

        # All entries here have been preprocessed earlier
        # sample   {DUMMY8, "<PTH>Dummy8\n<FRM>uint8"},
        itemPath = item['SBEM_PTH']
        lastDotIdx=itemPath.rfind('.')
        itemPath = list(itemPath)

        if isFirstInArray:
            itemPath[lastDotIdx] = '+'
        if isFirstInRootGroup:
            grpId = rootGroupStartItems[item['id']]['id']
            # grpId is of style "MEAS_ACC_SAMPLERATE_", so we have to find position of second last _ and put + there in itemPath
            plusPosition = grpId.rfind('_', 0, len(grpId)-1)
            if plusPosition>=0:
                itemPath[plusPosition] = '+'
            else:
                itemPath.insert(0,'+') # deals with one piece path e.g. "/MyService/Subscription"

        # Set primitive item values (e.g. "/Time") arrayable
        if (item['__property'] == '/'):
            itemPath += '+'

        from sbemCodeGen import dataLoggerConfig
        conversionConf = seekConversionConfig(dataLoggerConfig, "resources", "/" + item["id"].replace("_","/"), True)

        itemPath = "".join(itemPath)
        entry = ""
        descriptorItemIds.append(item['id'])

        itemFormat = item['SBEM_FRM']
        modifier = ""
        if conversionConf is not None:
            paramName = itemPath.replace("+",".").split(".")[-1]
            if paramName in conversionConf:
                if "to" in conversionConf[paramName]:
                    itemFormat = conversionConf[paramName]["to"]
                if "encoding" in conversionConf[paramName] and "decoding" in conversionConf[paramName]:
                    modifier = "<MOD>" + conversionConf[paramName]["decoding"].replace(" ","").replace("value","x")
                    modifier += "," +  conversionConf[paramName]["encoding"].replace(" ","").replace("value","y")
                    # add macros for encoding and decoding
                    encoderMacro = "#define ENCODE_" + item["id"] + "(value) (" + conversionConf[paramName]["encoding"] + ")"
                    decoderMacro = "#define DECODE_" + item["id"] + "(value) (" + conversionConf[paramName]["decoding"] + ")"
                    print(encoderMacro, file=hpp_file)
                    print(decoderMacro, file=hpp_file)
                    

        entry += '    {' + item['id'] + ", "
        entry += '\"'
        entry += "<PTH>" + itemPath
        entry += "\\n"
        entry += "<FRM>" + itemFormat
        if isNillable:
            entry += ",nillable=" + sbemNullValueFromItem(item)
        entry += modifier
        entry += '\"},'
        count += 1
        print(entry, file=cpp_file)

        #print("  generateDescriptorArray", itemPath, item['SBEM_FRM'])

    # ARRAY_BEGIN & END tags
    print('    {ARRAY_BEGIN, \"<PTH>[\"},', file=cpp_file)
    print('    {ARRAY_END, \"<PTH>]\"},', file=cpp_file)
    count += 2
    # end SBEM DescriptionItems array
    print('    {ID_DESCRIPTOR_ARRAY_END_MARKER, NULL}', file=cpp_file)
    print("};", file=cpp_file)
    return count, descriptorItemIds, itemIndexes

def generateWriteMethodCodeForItem(item, writeCodeArr, recursionLevel=1):
    global loopNumber    # Needed to modify global copy of loopNumber
    # Make sure that if item is marked as unsupported, we don't create code for it
    if writeCodeArr == None:
        return False
    if (('__INSERT_ASSERT' in item) and item['__INSERT_ASSERT']):
        print("__INSERT_ASSERT detected: ", item)
        del writeCodeArr[:] # clear all code and return false
        return False

    isArray = '__isArray' in item and item['__isArray']
    methodCode_ForEnd = None
    methodCode_ForBegin = None
    indent = ' ' * 4*loopNumber
    if isArray:
        valExpr = valueExprFromItem(item, False)

        #print("handling writer code for Array: ", valExpr)

        # write for loop for traversing array
        loopVariable = "loopidx_" + str(loopNumber)
        loopNumber = loopNumber+1

        methodCode_ForBegin = indent + 'for(size_t '+loopVariable+'=0; '+loopVariable+'<'+valExpr+'.size(); '+loopVariable+'++)\n    {\n'
        methodCode_ForEnd = indent+'}\n'
        writeCodeArr.append(methodCode_ForBegin)


    writeMethodCode = ""
    if 'SBEM_FRM' in item:
        # write code for individual item (e.g. int64)
        frm = item['SBEM_FRM']

        valExpr = valueExprFromItem(item)

        #print("-----> ", frm) # datatype format

        from sbemCodeGen import dataLoggerConfig
        conf = seekConversionConfig(dataLoggerConfig, "resources", "/" + item["id"].replace("_","/"), True)

        valType = "auto"
        castOper = ""

        if conf is not None:
            paramName = valExpr.split(".")[-1]
            if paramName in conf:
                #print(paramName + " --------> CONV " + str(conf))
                if "to" in conf[paramName]:
                    valType = conf[paramName]["to"]
                    castOper = "static_cast<" + valType + ">"
                if "encoding" in conf[paramName]:
                    # replace the value expression with the encoded one
                    valExpr = conf[paramName]["encoding"].replace("value",valExpr)

            if 0: # debug
                print(item["id"] + ": " + pythonObjToHumanReadable(conf) + "\n")

        writeMethodCode += indent+'{\n'
        writeMethodCode += indent+'    ' + valType + ' value = '+ castOper +'(' + valExpr + ");\n"
        writeMethodCode += indent+'    const uint8_t *src = reinterpret_cast<const uint8_t*>(&value);\n'
        writeMethodCode += indent+'    for(size_t i=0; i<sizeof(value); i++, src++, bytesRead++)\n'
        writeMethodCode += indent+'        if ((bytesRead >= startOffset) && (bytesWritten < bufferLen)) byteBuf[bytesWritten++] = *src;\n'
        writeMethodCode += indent+'}\n'

    elif 'subgroups' in item and item['subgroups']:
        # write for loop for traversing array
        for grp in item['subgroups']:
            #print("handling writer code for subgroup: ", grp)
            generateWriteMethodCodeForItem(grp, writeCodeArr)

    else:
        print("generateWriteMethodCodeForItem NOT IMPLEMENTED: ", item)

    writeCodeArr.append(writeMethodCode)

    if methodCode_ForEnd:
        writeCodeArr.append(methodCode_ForEnd)


    return True


# recursive function to greate groups from complex datatypes
def generateGroupsForResAndDatatype(defs, resource, path, datatype, resourceGroups, resourceItems):
    #print ("generateGroupsForResAndDatatype(). path: ",  path, " datatype: ", datatype['__datatype'])

    if not 'properties' in datatype:
#        print("No properties in datatype: ", datatype)
        return

    subGroups = []
    subItems = []

    # do each property, recurse into complex ones
    for k,v in sorted(datatype['properties'].iteritems()):

        if not 'SBEM_KIND' in v:
            print(" ERROR! v:",v)
            raise Exception("KIND MISSING!")

        sbem_log_enabled = True # default is enabled, may be disabled in yaml

        kind = v['SBEM_KIND']
        if 'simple' == kind:

            propPath = path + k

            longPath = resource['SBEM_PTH'] + propPath

            # for simple properties make entry that is simple item
            # make sure that array indicators don't end up in sbem path

            itemSubGroup = {'id': cppNameFromResName(longPath),
                         'SBEM_FRM': v['SBEM_FRM'],
                         'SBEM_PTH': resourceToSbemPath(longPath),
                         '__property': k,
                         '__required': v['__required']
                         }

            subItems.append(itemSubGroup)
            resourceItems.append(itemSubGroup)

        # do complex properties recursively
        elif 'complex' == kind:

            propPath = path + k + "/"
            #print("complex propPath: ", propPath)

            propGroup = generateGroupsForResAndDatatype(defs,
                                            resource,
                                            propPath,
                                            defs[v['__datatype']],
                                            resourceGroups,
                                            resourceItems)
            propGroup['__required'] = v['__required']

            subGroups.append(propGroup)
            resourceGroups.append(propGroup)

        elif 'array' == kind:
            #print('array kind: ', k)
            # find what kind of items the array has

            itemdef = v['items']
            if '$ref' in itemdef:
                datatypename = itemdef['$ref'].split('/')[-1]
                #print('  - datatypename: ', datatypename)

                propPath = path + k + "/"

                itemsGroup = generateGroupsForResAndDatatype(defs,
                                                resource,
                                                propPath,
                                                defs[datatypename],
                                                resourceGroups,
                                                resourceItems)

                if itemsGroup:
                    # Array items are required
                    itemsGroup['__required'] = True

                    # Mark "array" here
                    itemsGroup['__isArray'] = True

                    subGroups.append(itemsGroup)
                    resourceGroups.append(itemsGroup)
                    #print("itemsGroup for array: ", itemsGroup)


            elif 'type' in itemdef:
                # get FRM def from itemdef (won't be in array type property)
                frmString = frmFromSchema(itemdef) 

                propPath = path + k
                longPath = resource['SBEM_PTH'] + propPath
                itemSubGroup = {'id': cppNameFromResName(longPath),
                                'SBEM_FRM': frmString,
                                'SBEM_PTH': resourceToSbemPath(longPath),
                                '__property': k,
                                '__isArray': True,
                                '__required': True
                         }
                #print("simple array: ", itemSubGroup)
                subItems.append(itemSubGroup)
                resourceItems.append(itemSubGroup)

            else:
                raise Exception("Weird itemdef encountered: ", itemdef)

        else:
            raise Exception("Unknown SBEM_KIND encountered:", kind)

    # make a parent group
    longPath = resource['SBEM_PTH'] + path
    required = path=='/' 

    # Make sure that the first item is not an array (causes bad json after conversion)
    if len(subItems) > 1 and '__isArray' in subItems[0] and subItems[0]['__isArray']:
        firstNonArrayIdx = None
        for i,item in enumerate(subItems):
            if not '__isArray' in subItems[i] or not subItems[i]['__isArray']:
                firstNonArrayIdx = i
        if firstNonArrayIdx:
            list_prefix = subItems[:firstNonArrayIdx]
            item = subItems[firstNonArrayIdx]
            list_postfix = subItems[firstNonArrayIdx+1:]
            subItems = [ item ] + list_prefix + list_postfix

    complexGroup = {'id': cppNameFromResName(longPath),
                    'subgroups': subItems + subGroups,
                    '__property': path,
# required flag = true for root datatypes
                    '__required': required }

    # store link upward to current group
    for subgrp in complexGroup['subgroups']:
        subgrp['__parentGrp'] = complexGroup

    # return the group for this path
    return complexGroup
