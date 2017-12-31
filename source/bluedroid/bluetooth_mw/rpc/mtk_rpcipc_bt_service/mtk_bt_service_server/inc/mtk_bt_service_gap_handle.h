/********************************************************************************************
 *     LEGAL DISCLAIMER
 *
 *     (Header of MediaTek Software/Firmware Release or Documentation)
 *
 *     BY OPENING OR USING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 *     THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE") RECEIVED
 *     FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON AN "AS-IS" BASIS
 *     ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES, EXPRESS OR IMPLIED,
 *     INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR
 *     A PARTICULAR PURPOSE OR NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY
 *     WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY,
 *     INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK
 *     ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
 *     NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S SPECIFICATION
 *     OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
 *
 *     BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE LIABILITY WITH
 *     RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE, AT MEDIATEK'S OPTION,
 *     TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE
 *     FEES OR SERVICE CHARGE PAID BY BUYER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 *     THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE WITH THE LAWS
 *     OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF LAWS PRINCIPLES.
 ************************************************************************************************/

#ifndef _MTK_BT_SERVICE_GAP_HANDLE_H_
#define _MTK_BT_SERVICE_GAP_HANDLE_H_

#include "u_rpcipc_types.h"

typedef enum
{
    APP_EVENT_CB_IDX = 0,
    APP_GAP_GET_PAIRING_KEY_CB_IDX,
    APP_A2DP_UPLOAD_INFO_CB_IDX,
    APP_A2DP_PLAYBACK_CTRL_CB_IDX,
    APP_A2DP_RESET_AUDIO_CB_IDX,
    APP_AVRCP_PLAY_STATE_CHANGE_CB_IDX,
    APP_AVRCP_VOLUME_CHANGE_CB_IDX,
    APP_AVRCP_TRACK_CHANGE_CB_IDX,
    APP_AVRCP_MEDIA_STATUS_CB_IDX,
    APP_AVRCP_ABSOLUTE_VOLUME_CB_IDX,
    APP_AVRCP_CMD_CB_IDX,
    APP_AVRCP_GET_PLAY_STATUS_CB_IDX,
    APP_HID_CB_IDX,
    APP_SPP_CB_IDX,
    APP_HFCLIENT_CB_IDX,
    APP_BOND_CB_IDX,
    APP_CB_IDX_NUM,
} app_cb_idx;

extern INT32 c_rpc_reg_mtk_bt_service_gap_op_hndlrs(VOID);

#endif