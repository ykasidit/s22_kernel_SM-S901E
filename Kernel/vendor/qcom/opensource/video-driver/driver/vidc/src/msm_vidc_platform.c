// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2020-2021, The Linux Foundation. All rights reserved.
 */

#include <linux/of_platform.h>
#include "msm_vidc_platform.h"
#include "msm_vidc_debug.h"
#include "msm_vidc_v4l2.h"
#include "msm_vidc_vb2.h"
#include "msm_vidc_control.h"
#include "msm_vidc_waipio.h"
#include "msm_vidc_iris2.h"

static struct v4l2_file_operations msm_v4l2_file_operations = {
	.owner                          = THIS_MODULE,
	.open                           = msm_v4l2_open,
	.release                        = msm_v4l2_close,
	.unlocked_ioctl                 = video_ioctl2,
	.poll                           = msm_v4l2_poll,
};

static struct v4l2_ioctl_ops msm_v4l2_ioctl_ops_enc = {
	.vidioc_querycap                = msm_v4l2_querycap,
	.vidioc_enum_fmt_vid_cap        = msm_v4l2_enum_fmt,
	.vidioc_enum_fmt_vid_out        = msm_v4l2_enum_fmt,
	.vidioc_enum_framesizes         = msm_v4l2_enum_framesizes,
	.vidioc_enum_frameintervals     = msm_v4l2_enum_frameintervals,
	.vidioc_try_fmt_vid_cap_mplane  = msm_v4l2_try_fmt,
	.vidioc_try_fmt_vid_out_mplane  = msm_v4l2_try_fmt,
	.vidioc_s_fmt_vid_cap           = msm_v4l2_s_fmt,
	.vidioc_s_fmt_vid_out           = msm_v4l2_s_fmt,
	.vidioc_s_fmt_vid_cap_mplane    = msm_v4l2_s_fmt,
	.vidioc_s_fmt_vid_out_mplane    = msm_v4l2_s_fmt,
	.vidioc_s_fmt_meta_out          = msm_v4l2_s_fmt,
	.vidioc_s_fmt_meta_cap          = msm_v4l2_s_fmt,
	.vidioc_g_fmt_vid_cap           = msm_v4l2_g_fmt,
	.vidioc_g_fmt_vid_out           = msm_v4l2_g_fmt,
	.vidioc_g_fmt_vid_cap_mplane    = msm_v4l2_g_fmt,
	.vidioc_g_fmt_vid_out_mplane    = msm_v4l2_g_fmt,
	.vidioc_g_fmt_meta_out          = msm_v4l2_g_fmt,
	.vidioc_g_fmt_meta_cap          = msm_v4l2_g_fmt,
	.vidioc_g_selection             = msm_v4l2_g_selection,
	.vidioc_s_selection             = msm_v4l2_s_selection,
	.vidioc_s_parm                  = msm_v4l2_s_parm,
	.vidioc_g_parm                  = msm_v4l2_g_parm,
	.vidioc_reqbufs                 = msm_v4l2_reqbufs,
	.vidioc_qbuf                    = msm_v4l2_qbuf,
	.vidioc_dqbuf                   = msm_v4l2_dqbuf,
	.vidioc_streamon                = msm_v4l2_streamon,
	.vidioc_streamoff               = msm_v4l2_streamoff,
	.vidioc_s_ctrl                  = msm_v4l2_s_ctrl,
	.vidioc_g_ctrl                  = msm_v4l2_g_ctrl,
	.vidioc_queryctrl               = msm_v4l2_queryctrl,
	.vidioc_querymenu               = msm_v4l2_querymenu,
	.vidioc_subscribe_event         = msm_v4l2_subscribe_event,
	.vidioc_unsubscribe_event       = msm_v4l2_unsubscribe_event,
	.vidioc_encoder_cmd             = msm_v4l2_encoder_cmd,
};

static struct v4l2_ioctl_ops msm_v4l2_ioctl_ops_dec = {
	.vidioc_querycap                = msm_v4l2_querycap,
	.vidioc_enum_fmt_vid_cap        = msm_v4l2_enum_fmt,
	.vidioc_enum_fmt_vid_out        = msm_v4l2_enum_fmt,
	.vidioc_enum_framesizes         = msm_v4l2_enum_framesizes,
	.vidioc_enum_frameintervals     = msm_v4l2_enum_frameintervals,
	.vidioc_try_fmt_vid_cap_mplane  = msm_v4l2_try_fmt,
	.vidioc_try_fmt_vid_out_mplane  = msm_v4l2_try_fmt,
	.vidioc_s_fmt_vid_cap           = msm_v4l2_s_fmt,
	.vidioc_s_fmt_vid_out           = msm_v4l2_s_fmt,
	.vidioc_s_fmt_vid_cap_mplane    = msm_v4l2_s_fmt,
	.vidioc_s_fmt_vid_out_mplane    = msm_v4l2_s_fmt,
	.vidioc_s_fmt_meta_out          = msm_v4l2_s_fmt,
	.vidioc_s_fmt_meta_cap          = msm_v4l2_s_fmt,
	.vidioc_g_fmt_vid_cap           = msm_v4l2_g_fmt,
	.vidioc_g_fmt_vid_out           = msm_v4l2_g_fmt,
	.vidioc_g_fmt_vid_cap_mplane    = msm_v4l2_g_fmt,
	.vidioc_g_fmt_vid_out_mplane    = msm_v4l2_g_fmt,
	.vidioc_g_fmt_meta_out          = msm_v4l2_g_fmt,
	.vidioc_g_fmt_meta_cap          = msm_v4l2_g_fmt,
	.vidioc_g_selection             = msm_v4l2_g_selection,
	.vidioc_s_selection             = msm_v4l2_s_selection,
	.vidioc_s_parm                  = msm_v4l2_s_parm,
	.vidioc_g_parm                  = msm_v4l2_g_parm,
	.vidioc_reqbufs                 = msm_v4l2_reqbufs,
	.vidioc_qbuf                    = msm_v4l2_qbuf,
	.vidioc_dqbuf                   = msm_v4l2_dqbuf,
	.vidioc_streamon                = msm_v4l2_streamon,
	.vidioc_streamoff               = msm_v4l2_streamoff,
	.vidioc_s_ctrl                  = msm_v4l2_s_ctrl,
	.vidioc_g_ctrl                  = msm_v4l2_g_ctrl,
	.vidioc_queryctrl               = msm_v4l2_queryctrl,
	.vidioc_querymenu               = msm_v4l2_querymenu,
	.vidioc_subscribe_event         = msm_v4l2_subscribe_event,
	.vidioc_unsubscribe_event       = msm_v4l2_unsubscribe_event,
	.vidioc_decoder_cmd             = msm_v4l2_decoder_cmd,
};

static struct v4l2_ctrl_ops msm_v4l2_ctrl_ops = {
	.s_ctrl                         = msm_v4l2_op_s_ctrl,
};

static struct vb2_ops msm_vb2_ops = {
	.queue_setup                    = msm_vidc_queue_setup,
	.start_streaming                = msm_vidc_start_streaming,
	.buf_queue                      = msm_vidc_buf_queue,
	.buf_cleanup                    = msm_vidc_buf_cleanup,
	.stop_streaming                 = msm_vidc_stop_streaming,
};

static struct vb2_mem_ops msm_vb2_mem_ops = {
	.get_userptr                    = msm_vb2_get_userptr,
	.put_userptr                    = msm_vb2_put_userptr,
	.attach_dmabuf                  = msm_vb2_attach_dmabuf,
	.detach_dmabuf                  = msm_vb2_detach_dmabuf,
	.map_dmabuf                     = msm_vb2_map_dmabuf,
	.unmap_dmabuf                   = msm_vb2_unmap_dmabuf,
};

static int msm_vidc_init_ops(struct msm_vidc_core *core)
{
	if (!core) {
		d_vpr_e("%s: invalid params\n", __func__);
		return -EINVAL;
	}

	d_vpr_h("%s: initialize ops\n", __func__);
	core->v4l2_file_ops = &msm_v4l2_file_operations;
	core->v4l2_ioctl_ops_enc = &msm_v4l2_ioctl_ops_enc;
	core->v4l2_ioctl_ops_dec = &msm_v4l2_ioctl_ops_dec;
	core->v4l2_ctrl_ops = &msm_v4l2_ctrl_ops;
	core->vb2_ops = &msm_vb2_ops;
	core->vb2_mem_ops = &msm_vb2_mem_ops;

	return 0;
}

static int msm_vidc_deinit_platform_variant(struct msm_vidc_core *core, struct device *dev)
{
	int rc = 0;

	if (!core || !dev) {
		d_vpr_e("%s: Invalid params\n", __func__);
		return -EINVAL;
	}

	d_vpr_h("%s()\n", __func__);

	if (of_device_is_compatible(dev->of_node, "qcom,msm-vidc-waipio")) {
		rc = msm_vidc_deinit_platform_waipio(core, dev);
	} else {
		d_vpr_e("%s(): unknown platform\n", __func__);
		rc = -EINVAL;
	}

	if (rc)
		d_vpr_e("%s: failed with %d\n", __func__, rc);

	return rc;
}

static int msm_vidc_init_platform_variant(struct msm_vidc_core *core, struct device *dev)
{
	int rc = 0;

	if (!core || !dev) {
		d_vpr_e("%s: Invalid params\n", __func__);
		return -EINVAL;
	}

	d_vpr_h("%s()\n", __func__);

	if (of_device_is_compatible(dev->of_node, "qcom,msm-vidc-waipio")) {
		rc = msm_vidc_init_platform_waipio(core, dev);
	} else {
		d_vpr_e("%s(): unknown platform\n", __func__);
		rc = -EINVAL;
	}

	if (rc)
		d_vpr_e("%s: failed with %d\n", __func__, rc);

	return rc;
}

static int msm_vidc_deinit_vpu(struct msm_vidc_core *core, struct device *dev)
{
	int rc = 0;

	if (!core || !dev) {
		d_vpr_e("%s: Invalid params\n", __func__);
		return -EINVAL;
	}

	d_vpr_h("%s()\n", __func__);

	if (of_device_is_compatible(dev->of_node, "qcom,msm-vidc-iris2")) {
		rc = msm_vidc_deinit_iris2(core);
	} else {
		d_vpr_e("%s(): unknown vpu\n", __func__);
		rc = -EINVAL;
	}

	if (rc)
		d_vpr_e("%s: failed with %d\n", __func__, rc);

	return rc;
}

static int msm_vidc_init_vpu(struct msm_vidc_core *core, struct device *dev)
{
	int rc = 0;

	if (!core || !dev) {
		d_vpr_e("%s: Invalid params\n", __func__);
		return -EINVAL;
	}

	d_vpr_h("%s()\n", __func__);

	if (of_device_is_compatible(dev->of_node, "qcom,msm-vidc-iris2")) {
		rc = msm_vidc_init_iris2(core);
	} else {
		d_vpr_e("%s(): unknown vpu\n", __func__);
		rc = -EINVAL;
	}

	if (rc)
		d_vpr_e("%s: failed with %d\n", __func__, rc);

	return rc;
}

int msm_vidc_deinit_platform(struct platform_device *pdev)
{
	struct msm_vidc_core *core;

	if (!pdev) {
		d_vpr_e("%s: invalid params\n", __func__);
		return -EINVAL;
	}

	core = dev_get_drvdata(&pdev->dev);
	if (!core) {
		d_vpr_e("%s: core not found in device %s",
			dev_name(&pdev->dev));
		return -EINVAL;
	}

	d_vpr_h("%s()\n", __func__);

	msm_vidc_deinit_vpu(core, &pdev->dev);
	msm_vidc_deinit_platform_variant(core, &pdev->dev);

	kfree(core->platform);
	return 0;
}

int msm_vidc_init_platform(struct platform_device *pdev)
{
	int rc = 0;
	struct msm_vidc_platform *platform;
	struct msm_vidc_core *core;

	if (!pdev) {
		d_vpr_e("%s: invalid params\n", __func__);
		return -EINVAL;
	}

	d_vpr_h("%s()\n", __func__);

	core = dev_get_drvdata(&pdev->dev);
	if (!core) {
		d_vpr_e("%s: core not found in device %s",
			dev_name(&pdev->dev));
		return -EINVAL;
	}

	platform = kzalloc(sizeof(struct msm_vidc_platform), GFP_KERNEL);
	if (!platform)
		return -ENOMEM;

	core->platform = platform;
	platform->core = core;

	/* selected ops can be re-assigned in platform specific file */
	rc = msm_vidc_init_ops(core);
	if (rc)
		return rc;

	rc = msm_vidc_init_platform_variant(core, &pdev->dev);
	if (rc)
		return rc;

	rc = msm_vidc_init_vpu(core, &pdev->dev);
	if (rc)
		return rc;

	return rc;
}
