// Copyright (c) Suunto Oy 2017. All rights reserved.
#pragma once

#include <whiteboard/LaunchableModule.h>
#include <whiteboard/ResourceProvider.h>
#include "buildconfig/movesense-core/features.h"
#include "hal/manufacturingdata/manufacturingdata.h"
#include "common/compiler/genType.h"

#define STEPDATA_OFFSET         0x70B00
#define CALIBRATIONDATA_OFFSET  0x70500
#define PRODUCTDATA_OFFSET      0x70000

#define PRODUCTDATA_SIZE        0x01000     // 4kB

#define ERROR_NO_DATA           -2
#define ERROR_NOT_EMPTY         -1

#define PD_MNFR_NAME_LEN        32
#define PD_PRODUCT_NAME_LEN     64
#define PD_VARIANT_LEN          16
#define PD_DESIGN_LEN           32
#define PD_DESIGN_LEN           32
#define PD_HW_COMP_LEN          8
#define PD_SERIAL_LEN           12
#define PD_PCBA_SERIAL_LEN      64
#define PD_HW_LEN               16


// Derive ProducdData struct address from manufacturingdata.h,
#define PRODUCT_DATA_ADDRESS 0x74C00

#define PRODUCT_DATA_PTR ((ProductData *) PRODUCT_DATA_ADDRESS)



class ManufacturingService FINAL :
    private whiteboard::ResourceProvider,
    public whiteboard::LaunchableModule

{
public:
    /** Name of this class. Used in StartupProvider list. */
    static const char_t* const LAUNCHABLE_NAME;
    ManufacturingService();
    ~ManufacturingService();

private:
    /** @see whiteboard::ILaunchableModule::initModule */
    virtual bool initModule() OVERRIDE;

    /** @see whiteboard::ILaunchableModule::deinitModule */
    virtual void deinitModule() OVERRIDE;

    /** @see whiteboard::ILaunchableModule::startModule */
    virtual bool startModule() OVERRIDE;

    /** @see whiteboard::ILaunchableModule::stopModule */
    virtual void stopModule() OVERRIDE { mModuleState = WB_RES::ModuleStateValues::STOPPED; }

    void ProductDataPost(const whiteboard::Request& request,
            const whiteboard::ParameterList& parameters);

    void ProductDataGet(const whiteboard::Request& request,
            const whiteboard::ParameterList& parameters);

    void ProductDataDelete(const whiteboard::Request& request,
                           const whiteboard::ParameterList& parameters);

    void CalibrationDataPost(const whiteboard::Request& request,
            const whiteboard::ParameterList& parameters);

    void CalibrationDataGet(const whiteboard::Request& request,
            const whiteboard::ParameterList& parameters);

    void CalibrationDataDelete(const whiteboard::Request& request,
            const whiteboard::ParameterList& parameters);

    void StepDataPost(const whiteboard::Request& request,
            const whiteboard::ParameterList& parameters);

    void StepDataGet(const whiteboard::Request& request,
            const whiteboard::ParameterList& parameters);

    void StepDataDelete(const whiteboard::Request& request,
            const whiteboard::ParameterList& parameters);

    void BatteryCorrectionPost(const whiteboard::Request& request, const whiteboard::ParameterList& parameters);
    void BatteryCorrectionGet(const whiteboard::Request& request, const whiteboard::ParameterList& parameters);
    void BatteryCorrectionDelete(const whiteboard::Request& request, const whiteboard::ParameterList& parameters);

#ifdef BUILD_APP_FEATURE_AFE_CALIBRATION
    void AfeCorrectionPost(const whiteboard::Request& request, const whiteboard::ParameterList& parameters);
    void AfeCorrectionGet(const whiteboard::Request& request, const whiteboard::ParameterList& parameters);
    void AfeCorrectionDelete(const whiteboard::Request& request, const whiteboard::ParameterList& parameters);
#endif // BUILD_APP_FEATURE_AFE_CALIBRATION

    virtual void onGetRequest(const whiteboard::Request& request,
                              const whiteboard::ParameterList& parameters) OVERRIDE;

    virtual void onPostRequest(const whiteboard::Request& request,
                              const whiteboard::ParameterList& parameters) OVERRIDE;

    virtual void onDeleteRequest(const whiteboard::Request& request,
                              const whiteboard::ParameterList& parameters) OVERRIDE;


    bool isEmpty(uint32_t *data, size_t size);
    bool isEmpty(uint32_t offset, size_t size);

    void notData(uint32_t *data, size_t size);
    bool eraseManufacturingFlashPage();

    /**
    *	Checks if the given S/N is correct by parsing all its required fields and by comparing to their ranges
    *   All of the range values are taken from the "Suunto Twelve Digit Serial Number" documentation V0.4 09.11.2015
    *
    *   @return Boolean status of serial number correctness
    */
    bool isSerialNumberCorrect(whiteboard::WrapperFor32BitPointer<const char> serialNumberStringToVerify);

    /**
    *	Checks if unique ID (which is a substring of S/N) is correct by cheking the range of all of its digits one by one
    *   All of the range values are taken from the "Suunto Twelve Digit Serial Number" documentation V0.4 09.11.2015
    *
    *   @return Boolean status of devices unique ID correctness
    */
    bool isUniqueIDCorrect(const char* uniqueIDString);

    uint32_t stepStringToEnum(const char *string);
    void stepEnumToString(char* buffer, size_t buffer_size, const uint32_t &value);
};
