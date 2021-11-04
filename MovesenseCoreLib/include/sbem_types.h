#pragma once

#include <stdint.h>
#include "whiteboard/Identifiers.h"
#include "whiteboard/Value.h"
#include "common/core/debug.h"

#define CREATE_GROUP(name, ...)                                                                    \
    static const uint16 name##_items_items[] = {__VA_ARGS__};                                             \
    const DescriptorGroup_t name##_Group = {                                                       \
        name,                                                                              \
        ELEMENTS(name##_items_items),                                                              \
        name##_items_items                                                                         \
    }
#define DECLARE_GROUP(name)                                                                    \
    extern const DescriptorGroup_t name##_Group;

enum CommonValueIds_e {
    DESCRIPTOR = 0, // 0 reserved for descriptor chunks.
    SOURCE = 1, // Descriptors start from id 1
    TIMEISO8601,
    COMMON_DESCRIPTORS_COUNT
};


constexpr uint16_t ID_DESCRIPTOR_ARRAY_END_MARKER = 0xFFFFu;
constexpr uint16_t MAX_DESCRIPTORS_COUNT = 80;
constexpr uint16_t MAX_DESCRIPTOR_LENGTH = 256;

struct DescriptorItem_t {
    int id;
    const char* path;
};

struct DescriptorGroup_t {
    uint16_t id;
    uint16_t numOfValueIds;
    const uint16_t* valueIds;
};

struct SbemResID2GrpIdMapping {
    const whiteboard::LocalResourceId wbResId;
    const size_t sbemBytes;
    const DescriptorGroup_t *pSbemGrp;
};

struct SbemResID2ItemIdMapping {
    const whiteboard::LocalResourceId wbResId;
    const DescriptorItem_t *pSbemItem;
};

// These will be generated as a part of firmware build process
extern const DescriptorItem_t s_possibleSbemItems[];
extern const DescriptorGroup_t s_possibleSbemGroups[];
extern const SbemResID2ItemIdMapping s_sbemResID2ItemIdMap[];
extern const SbemResID2GrpIdMapping s_sbemResID2GrpIdMap[];

constexpr SbemResID2ItemIdMapping ITEM_MAPPING_END_MARKER = {wb::ID_INVALID_LOCAL_RESOURCE, nullptr};
constexpr SbemResID2GrpIdMapping GROUP_MAPPING_END_MARKER = {wb::ID_INVALID_LOCAL_RESOURCE, 0, nullptr};

typedef void (*SbemNodeVisited_t)(uint16_t nodeId, bool isGroup);

uint16_t traverseSbemDefinition(uint16_t startNodeId, SbemNodeVisited_t callback);

uint16_t getSbemDescriptorIdsFromResource(whiteboard::LocalResourceId localResourceId, uint16_t idsOut[],  size_t bufSize);
uint16_t getSbemDescriptorIdFromResource(whiteboard::LocalResourceId localResourceId, size_t dataLength);

size_t getSbemLength(whiteboard::LocalResourceId localResId, const whiteboard::Value &data);
int16_t getSbemLengthConst(whiteboard::LocalResourceId localResId);

size_t writeToSbemBuffer(void *buffer, size_t bufferLen, size_t startOffset, whiteboard::LocalResourceId localResId, const whiteboard::Value &data);

size_t writeSbemItemDescriptor(uint16_t itemId, void *buffer);
size_t writeSbemGroupDescriptor(uint16_t grpId, void *buffer);

uint16_t getSbemItemsCount();
uint16_t getSbemGroupsCount();
uint16_t getFirstSbemGroupId();

#define BUFFER_INDEX (pos - buffer)