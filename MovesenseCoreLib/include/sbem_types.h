#pragma once

#include <stdint.h>
#include "whiteboard/Identifiers.h"
#include "whiteboard/Value.h"
#include "common/core/debug.h"
#include "movesense.h"

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

struct DescriptorItem_t {
    const int id;
    const char* const path;
};

struct DescriptorGroup_t {
    const uint16_t id;
    const uint16_t numOfValueIds;
    const uint16_t* const valueIds;
};

struct SbemResID2GrpIdMapping {
    const whiteboard::LocalResourceId wbResId;
    const DescriptorGroup_t &sbemGrp;
};

struct SbemResID2ItemIdMapping {
    const whiteboard::LocalResourceId wbResId;
    const DescriptorItem_t &sbemItem;
};

uint16_t getSbemDescriptorIdFromResource(whiteboard::LocalResourceId localResourceId);

size_t getSbemLength(whiteboard::LocalResourceId localResId, const whiteboard::Value &data);
int16_t getSbemLengthConst(whiteboard::LocalResourceId localResId);

size_t writeToSbemBuffer(void *buffer, whiteboard::LocalResourceId localResId, const whiteboard::Value &data);

size_t writeSbemItemDescriptor(uint16_t itemId, void *buffer);
size_t writeSbemGroupDescriptor(uint16_t grpId, void *buffer);

uint16_t getSbemItemsCount();
uint16_t getSbemGroupsCount();
uint16_t getFirstSbemGroupId();

