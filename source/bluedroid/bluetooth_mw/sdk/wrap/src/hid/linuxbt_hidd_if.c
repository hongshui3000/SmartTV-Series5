#ifdef __CE_LINUX_SUPPORT__
#ifdef __BT_HIDD_PROFILE__  //CastTV HID Client

#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/prctl.h>
#include <pthread.h>

#include "bluetooth.h"
#include "bt_hd.h"
#include "u_bluetooth.h"

#include "linuxbt_hidd_if.h"
#include "linuxbt_common.h"
#include "bt_mw_common.h"
#include "bt_mw_hidd.h"

extern VOID *linuxbt_gap_get_profile_interface(const CHAR *profile_id);

static VOID linuxbt_hidd_connection_state_cb(bt_bdaddr_t *bd_addr, bthd_connection_state_t state)
{
    BT_DBG_NORMAL(BT_DEBUG_HID, "[HID_device] %s()", __FUNCTION__);
    switch (state) {
        case BTHD_CONN_STATE_CONNECTED:
            BT_DBG_NORMAL(BT_DEBUG_HID, "[HID_device] Connection State : '%d' connected", state);
            bluetooth_handle_hidd_connect_cb(bd_addr);
            break;
        case BTHD_CONN_STATE_DISCONNECTED:
            BT_DBG_NORMAL(BT_DEBUG_HID, "[HID_device] Connection State : '%d' disconnected", state);
            bluetooth_handle_hidd_disconnect_cb(bd_addr);
            break;

        case BTHD_CONN_STATE_CONNECT_FAILED:
            BT_DBG_NORMAL(BT_DEBUG_HID, "[HID_device] Connection State : '%d' connnect fail", state);
            bluetooth_handle_hidd_connect_fail_cb(bd_addr);
            break;
        case BTHD_CONN_STATE_UNKNOWN:
            BT_DBG_NORMAL(BT_DEBUG_HID, "[HID_device] Connection State : '%d' UNKNOWN", state);
            break;
        default:
            BT_DBG_NORMAL(BT_DEBUG_HID, "[HID_device] Connection State : '%d'", state);
    }
}

static VOID linuxbt_hidd_virtual_unplug_cb(bt_bdaddr_t *bd_addr, bthd_status_t hd_status)
{
    BT_DBG_NORMAL(BT_DEBUG_HID, "[HID_device] %s()", __FUNCTION__);
    switch (hd_status) {
        case BTHD_OK:
            BT_DBG_NORMAL(BT_DEBUG_HID, "[HID_device] Virtual unplug State : '%d' OK", hd_status);
            break;
        case BTHD_HS_HID_NOT_READY:  /* handshake error : device not ready */
            BT_DBG_NORMAL(BT_DEBUG_HID, "[HID_device] Virtual unplug State : '%d' HS_HID_NOT_READY", hd_status);
            break;
        case BTHD_HS_INVALID_RPT_ID:  /* handshake error : invalid report ID */
            BT_DBG_NORMAL(BT_DEBUG_HID, "[HID_device] Virtual unplug State : '%d' HS_INVALID_RPT_ID", hd_status);
            break;
        case BTHD_HS_TRANS_NOT_SPT:  /* handshake error : transaction not spt */
            BT_DBG_NORMAL(BT_DEBUG_HID, "[HID_device] Virtual unplug State : '%d' HS_TRANS_NOT_SPT", hd_status);
            break;
        case BTHD_HS_INVALID_PARAM:  /* handshake error : invalid paremter */
            BT_DBG_NORMAL(BT_DEBUG_HID, "[HID_device] Virtual unplug State : '%d' HS_INVALID_PARAM", hd_status);
            break;
        case BTHD_HS_ERROR:  /* handshake error : unspecified HS error */
            BT_DBG_NORMAL(BT_DEBUG_HID, "[HID_device] Virtual unplug State : '%d' HS_ERROR", hd_status);
            break;
        case BTHD_ERR:  /* general BTA HH error */
            BT_DBG_NORMAL(BT_DEBUG_HID, "[HID_device] Virtual unplug State : '%d' ERR", hd_status);
            break;
        case BTHD_ERR_SDP:  /* SDP error */
            BT_DBG_NORMAL(BT_DEBUG_HID, "[HID_device] Virtual unplug State : '%d' ERR_SDP", hd_status);
            break;
        case BTHD_ERR_PROTO:  /* SET_Protocol error, only used in BTA_HH_OPEN_EVT callback */
            BT_DBG_NORMAL(BT_DEBUG_HID, "[HID_device] Virtual unplug State : '%d' ERR_PROTO", hd_status);
            break;
        case BTHD_ERR_DB_FULL:  /* device database full error, used  */
            BT_DBG_NORMAL(BT_DEBUG_HID, "[HID_device] Virtual unplug State : '%d' ERR_DB_FULL", hd_status);
            break;
        case BTHD_ERR_TOD_UNSPT:  /* type of device not supported */
            BT_DBG_NORMAL(BT_DEBUG_HID, "[HID_device] Virtual unplug State : '%d' ERR_TOD_UNSPT", hd_status);
            break;
        case BTHD_ERR_NO_RES:  /* out of system resources */
            BT_DBG_NORMAL(BT_DEBUG_HID, "[HID_device] Virtual unplug State : '%d' ERR_NO_RES", hd_status);
            break;
        case BTHD_ERR_AUTH_FAILED:  /* authentication fail */
            BT_DBG_NORMAL(BT_DEBUG_HID, "[HID_device] Virtual unplug State : '%d' ERR_AUTH_FAILED", hd_status);
            break;
        case BTHD_ERR_HDL:
            BT_DBG_NORMAL(BT_DEBUG_HID, "[HID_device] Virtual unplug State : '%d' ERR_HDL", hd_status);
            break;
        default:
            BT_DBG_NORMAL(BT_DEBUG_HID, "[HID_device] Connection State : '%d'", hd_status);
    }
    // Do nothing for now.
}

static VOID linuxbt_hidd_send_data_cb(bt_bdaddr_t *bd_addr, bthd_status_t hd_status)
{
    BT_DBG_MINOR(BT_DEBUG_HID, "[HID_device] %s()", __FUNCTION__);
    switch (hd_status) {
        case BTHD_OK:
            BT_DBG_MINOR(BT_DEBUG_HID, "[HID_device] Send data State : '%d' OK", hd_status);
            break;
        case BTHD_HS_HID_NOT_READY:  /* handshake error : device not ready */
            BT_DBG_MINOR(BT_DEBUG_HID, "[HID_device] Send data State : '%d' HS_HID_NOT_READY", hd_status);
            break;
        case BTHD_HS_INVALID_RPT_ID:  /* handshake error : invalid report ID */
            BT_DBG_MINOR(BT_DEBUG_HID, "[HID_device] Send data State : '%d' HS_INVALID_RPT_ID", hd_status);
            break;
        case BTHD_HS_TRANS_NOT_SPT:  /* handshake error : transaction not spt */
            BT_DBG_MINOR(BT_DEBUG_HID, "[HID_device] Send data State : '%d' HS_TRANS_NOT_SPT", hd_status);
            break;
        case BTHD_HS_INVALID_PARAM:  /* handshake error : invalid paremter */
            BT_DBG_MINOR(BT_DEBUG_HID, "[HID_device] Send data State : '%d' HS_INVALID_PARAM", hd_status);
            break;
        case BTHD_HS_ERROR:  /* handshake error : unspecified HS error */
            BT_DBG_MINOR(BT_DEBUG_HID, "[HID_device] Send data State : '%d' HS_ERROR", hd_status);
            break;
        case BTHD_ERR:  /* general BTA HH error */
            BT_DBG_MINOR(BT_DEBUG_HID, "[HID_device] Send data State : '%d' ERR", hd_status);
            break;
        case BTHD_ERR_SDP:  /* SDP error */
            BT_DBG_MINOR(BT_DEBUG_HID, "[HID_device] Send data State : '%d' ERR_SDP", hd_status);
            break;
        case BTHD_ERR_PROTO:  /* SET_Protocol error, only used in BTA_HH_OPEN_EVT callback */
            BT_DBG_MINOR(BT_DEBUG_HID, "[HID_device] Send data State : '%d' ERR_PROTO", hd_status);
            break;
        case BTHD_ERR_DB_FULL:  /* device database full error, used  */
            BT_DBG_MINOR(BT_DEBUG_HID, "[HID_device] Send data State : '%d' ERR_DB_FULL", hd_status);
            break;
        case BTHD_ERR_TOD_UNSPT:  /* type of device not supported */
            BT_DBG_MINOR(BT_DEBUG_HID, "[HID_device] Send data State : '%d' ERR_TOD_UNSPT", hd_status);
            break;
        case BTHD_ERR_NO_RES:  /* out of system resources */
            BT_DBG_MINOR(BT_DEBUG_HID, "[HID_device] Send data State : '%d' ERR_NO_RES", hd_status);
            break;
        case BTHD_ERR_AUTH_FAILED:  /* authentication fail */
            BT_DBG_MINOR(BT_DEBUG_HID, "[HID_device] Send data State : '%d' ERR_AUTH_FAILED", hd_status);
            break;
        case BTHD_ERR_HDL:
            BT_DBG_MINOR(BT_DEBUG_HID, "[HID_device] Send data State : '%d' ERR_HDL", hd_status);
            break;
        default:
            BT_DBG_MINOR(BT_DEBUG_HID, "[HID_device] Connection State : '%d'", hd_status);
    }
    // Do nothing for now.
}

static bthd_interface_t *g_bt_hid_device_interface = NULL;
static bthd_callbacks_t g_bt_hid_device_callbacks =
{
    sizeof(bthd_callbacks_t),
    linuxbt_hidd_connection_state_cb,
    linuxbt_hidd_virtual_unplug_cb,
    linuxbt_hidd_send_data_cb,
};

#ifdef __CE_MTK_IF_API__
INT32 linuxbt_hidd_activate_handler(VOID)
{
    BT_DBG_NORMAL(BT_DEBUG_HID, "[HID_device] %s()", __FUNCTION__);
    if(g_bt_hid_device_interface == NULL)
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "[HID_device] Failed to get HID device interface");
        return -1;
    }

    g_bt_hid_device_interface->activate();

    return 0;
}

INT32 linuxbt_hidd_deactivate_handler(VOID)
{
    BT_DBG_NORMAL(BT_DEBUG_HID, "[HID_device] %s()", __FUNCTION__);
    if(g_bt_hid_device_interface == NULL)
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "[HID_device] Failed to get HID device interface");
        return -1;
    }

    g_bt_hid_device_interface->deactivate();

    return 0;
}
#endif

INT32 linuxbt_hidd_connect_handler(CHAR *pbt_addr)
{
    bt_bdaddr_t bdaddr;
    BT_DBG_NORMAL(BT_DEBUG_HID, "[HID_device] %s()", __FUNCTION__);
    if (NULL == pbt_addr)
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "null pointer of pbt_addr");
        return -1;
    }
    if(g_bt_hid_device_interface == NULL)
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "[HID_device] Failed to get HID device interface");
        return -1;
    }

    linuxbt_btaddr_stoh(pbt_addr, &bdaddr);
    BT_DBG_NORMAL(BT_DEBUG_HID, "HID Device connect to %02X:%02X:%02X:%02X:%02X:%02X",
        bdaddr.address[0], bdaddr.address[1], bdaddr.address[2],
        bdaddr.address[3], bdaddr.address[4], bdaddr.address[5]);

    g_bt_hid_device_interface->connect(&bdaddr);

    return 0;
}

INT32 linuxbt_hidd_disconnect_handler(CHAR *pbt_addr)
{
    bt_bdaddr_t bdaddr;
    BT_DBG_NORMAL(BT_DEBUG_HID, "[HID_device] %s()", __FUNCTION__);
    if(g_bt_hid_device_interface == NULL)
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "[HID_device] Failed to get HID device interface");
        return -1;
    }
    if (NULL == pbt_addr)
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "null pointer of pbt_addr");
        return -1;
    }
    linuxbt_btaddr_stoh(pbt_addr, &bdaddr);
    BT_DBG_NORMAL(BT_DEBUG_HID, "HID disconnect %02X:%02X:%02X:%02X:%02X:%02X",
        bdaddr.address[0], bdaddr.address[1], bdaddr.address[2],
        bdaddr.address[3], bdaddr.address[4], bdaddr.address[5]);

    g_bt_hid_device_interface->disconnect(&bdaddr);

    return 0;
}

INT32 linuxbt_hidd_virtual_unplug_handler(VOID)
{
    BT_DBG_NORMAL(BT_DEBUG_HID, "[HID_device] %s()", __FUNCTION__);

    g_bt_hid_device_interface->virtual_unplug();
    return 0;
}


INT32 linuxbt_hidd_send_data_handler(CHAR *data, INT32 dataSize)
{
    BT_DBG_MINOR(BT_DEBUG_HID, "[HID_device] %s", __FUNCTION__);

    g_bt_hid_device_interface->send_data(data, dataSize);
    return 0;
}

INT32 linuxbt_hidd_init(VOID)
{
    FUNC_ENTRY;
    INT32 ret = 0;

    // Get HID device interface
    g_bt_hid_device_interface = (bthd_interface_t *)linuxbt_gap_get_profile_interface(BT_PROFILE_HIDDEVICE_ID);
    if (g_bt_hid_device_interface == NULL)
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "[HID_device] Failed to get HID device interface");
        return -1;
    }

     // Init HID device interface
    if (g_bt_hid_device_interface->init(&g_bt_hid_device_callbacks) != BT_STATUS_SUCCESS)
    {
        BT_DBG_ERROR(BT_DEBUG_HID, "[HID_device] Failed to init HID device interface");
        return -1;
    }

    return ret;
}

INT32 linuxbt_hidd_deinit(VOID)
{
    FUNC_ENTRY;

    // Deinit HID device interface
    if (g_bt_hid_device_interface != NULL)
    {
        g_bt_hid_device_interface->cleanup();
    }

    return 0;
}

#endif
#else

#include "u_bluetooth.h"

INT32 linuxbt_hidd_activate_handler(VOID)
{
    return 0;
}

INT32 linuxbt_hidd_deactivate_handler(VOID)
{
    return 0;
}

INT32 linuxbt_hidd_connect_handler(CHAR *pbt_addr)
{
    return 0;
}

INT32 linuxbt_hidd_disconnect_handler(CHAR *pbt_addr)
{
    return 0;
}

INT32 linuxbt_hidd_virtual_unplug_handler(VOID)
{
    return 0;
}


INT32 linuxbt_hidd_send_data_handler(CHAR *data, INT32 dataSize)
{
    return 0;
}

INT32 linuxbt_hidd_init(VOID)
{
    return 0;
}

INT32 linuxbt_hidd_deinit(VOID)
{
    return 0;
}

#endif

