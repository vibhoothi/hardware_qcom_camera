/* Copyright (c) 2012-2013, The Linux Foundataion. All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are
* met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above
*       copyright notice, this list of conditions and the following
*       disclaimer in the documentation and/or other materials provided
*       with the distribution.
*     * Neither the name of The Linux Foundation nor the names of its
*       contributors may be used to endorse or promote products derived
*       from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
* ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
* BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
* BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
* IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/

#ifndef __QCAMERA3HARDWAREINTERFACE_H__
#define __QCAMERA3HARDWAREINTERFACE_H__

#include <pthread.h>
#include <utils/List.h>
#include <hardware/camera3.h>
#include <camera/CameraMetadata.h>
#include "QCamera3HALHeader.h"

extern "C" {
#include <mm_camera_interface.h>
#include <mm_jpeg_interface.h>
}

using namespace android;

namespace qcamera {

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

class QCamera3MetadataChannel;
class QCamera3HeapMemory;

class QCamera3HardwareInterface {
public:
    /* static variable and functions accessed by camera service */
    static camera3_device_ops_t mCameraOps;
    static int initialize(const struct camera3_device *,
                const camera3_callback_ops_t *callback_ops);
    static int configure_streams(const struct camera3_device *,
                camera3_stream_configuration_t *stream_list);
    static int register_stream_buffers(const struct camera3_device *,
                const camera3_stream_buffer_set_t *buffer_set);
    static const camera_metadata_t* construct_default_request_settings(
                                const struct camera3_device *, int type);
    static int process_capture_request(const struct camera3_device *,
                                camera3_capture_request_t *request);
    static void get_metadata_vendor_tag_ops(const struct camera3_device *,
                                               vendor_tag_query_ops_t* ops);
    static void dump(const struct camera3_device *, int fd);
    static int close_camera_device(struct hw_device_t* device);
public:
    QCamera3HardwareInterface(int cameraId);
    virtual ~QCamera3HardwareInterface();
    int openCamera(struct hw_device_t **hw_device);
    int getMetadata(int type);
    camera_metadata_t* translateCapabilityToMetadata(int type);

    static int getCamInfo(int cameraId, struct camera_info *info);
    static int initCapabilities(int cameraId);
    static int initStaticMetadata(int cameraId);
    static void makeTable(cam_dimension_t* dimTable, uint8_t size, int32_t* sizeTable);
    static void convertRegions(cam_rect_t rect, int32_t* region, int weight);
    static void convertLandmarks(cam_face_detection_info_t face, int32_t* landmarks);
    static int32_t getScalarFormat(int32_t format);

    static void captureResultCb(metadata_buffer_t *metadata,
                camera3_stream_buffer_t *buffer, uint32_t frame_number,
                void *userdata);

    void sendCaptureResult(const struct camera3_callback_ops *,
                        const camera3_capture_result_t *result);
    void notify(const struct camera3_callback_ops *,
                        const camera3_notify_msg_t *msg);

    int initialize(const camera3_callback_ops_t *callback_ops);
    int configureStreams(camera3_stream_configuration_t *stream_list);
    int registerStreamBuffers(const camera3_stream_buffer_set_t *buffer_set);
    int processCaptureRequest(camera3_capture_request_t *request);
    void getMetadataVendorTagOps(vendor_tag_query_ops_t* ops);
    void dump(int fd);

    int setFrameParameters(int frame_id, const camera_metadata_t *settings);
    int translateMetadataToParameters(const camera_metadata_t *settings);
    camera_metadata_t* translateCbMetadataToResultMetadata(metadata_buffer_t *metadata);
    int getJpegSettings(const camera_metadata_t *settings);
    int initParameters();

    void captureResultCb(metadata_buffer_t *metadata,
                camera3_stream_buffer_t *buffer, uint32_t frame_number);

    typedef struct {
        int fwk_name;
        int hal_name;
    } QCameraMap;

private:

    int openCamera();
    int closeCamera();
    int AddSetParmEntryToBatch(parm_buffer_t *p_table,
                               cam_intf_parm_type_t paramType,
                               uint32_t paramLength,
                               void *paramValue);
    static int lookupHalName(const QCameraMap arr[],
                      int len, int fwk_name);
    static int lookupFwkName(const QCameraMap arr[],
                      int len, int hal_name);

    int validateCaptureRequest(camera3_capture_request_t *request);

public:

private:
    camera3_device_t   mCameraDevice;
    uint8_t            mCameraId;
    mm_camera_vtbl_t  *mCameraHandle;
    bool               mCameraOpened;
    camera_metadata_t *mDefaultMetadata[CAMERA3_TEMPLATE_COUNT];

    const camera3_callback_ops_t *mCallbackOps;

    camera3_stream_t *mInputStream;
    QCamera3MetadataChannel *mMetadataChannel;

    QCamera3HeapMemory *mParamHeap;
    parm_buffer_t* mParameters;

    //mutex and conditional variable for request
    pthread_mutex_t mRequestLock;
    pthread_cond_t mRequestCond;
    int mPendingRequest;

    //mutex for serialized access to camera3_device_ops_t functions
    pthread_mutex_t mMutex;

    //mutex to protect the critial section for processCaptureResult
    pthread_mutex_t mCaptureResultLock;
    jpeg_settings_t* mJpegSettings;
    static const QCameraMap EFFECT_MODES_MAP[];
    static const QCameraMap WHITE_BALANCE_MODES_MAP[];
    static const QCameraMap SCENE_MODES_MAP[];
    static const QCameraMap FOCUS_MODES_MAP[];
    static const QCameraMap ANTIBANDING_MODES_MAP[];
    static const QCameraMap AUTO_EXPOSURE_MAP[];
    static const QCameraMap FLASH_MODES_MAP[];
};

}; // namespace qcamera

#endif /* __QCAMERA2HARDWAREINTERFACE_H__ */