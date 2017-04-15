ifeq ($(BOARD_BUILD_OP2_CAMERA),true)
ifneq ($(strip $(USE_DEVICE_SPECIFIC_CAMERA)),true)

MM_V4L2_DRIVER_LIST += msm8960
MM_V4L2_DRIVER_LIST += msm8974
MM_V4L2_DRIVER_LIST += msm8916
MM_V4L2_DRIVER_LIST += msm8226
MM_V4L2_DRIVER_LIST += msm8610
MM_V4L2_DRIVER_LIST += apq8084
MM_V4L2_DRIVER_LIST += mpq8092
MM_V4L2_DRIVER_LIST += msm_bronze
MM_V4L2_DRIVER_LIST += msm8916
MM_V4L2_DRIVER_LIST += msm8994
MM_V4L2_DRIVER_LIST += msm8084
MM_V4L2_DRIVER_LIST += msm8992

ifneq ($(strip $(USE_DEVICE_SPECIFIC_CAMERA)),true)
# TODO:  Find a better way to separate build configs for ADP vs non-ADP devices
ifneq ($(TARGET_BOARD_AUTO),true)
  ifneq ($(strip $(USE_CAMERA_STUB)),true)
    ifneq ($(BUILD_TINY_ANDROID),true)
      ifneq ($(filter msm8996,$(TARGET_BOARD_PLATFORM)),)
        include $(addsuffix /Android.mk, $(addprefix $(call my-dir)/, mm-image-codec QCamera2))
      endif
      ifneq ($(filter msm8998,$(TARGET_BOARD_PLATFORM)),)
        include $(call all-makefiles-under,$(call my-dir)/msm8998)
      endif
    endif
  endif
endif

endif
endif
