#ifndef __BTMW_TEST_DEBUG_H__
#define __BTMW_TEST_DEBUG_H__

#define BTMW_TEST_LOG_LV_VBS  (1 << 0)
#define BTMW_TEST_LOG_LV_INF  (1 << 1)
#define BTMW_TEST_LOG_LV_DBG  (1 << 2)
#define BTMW_TEST_LOG_LV_WRN  (1 << 3)
#define BTMW_TEST_LOG_LV_ERR  (1 << 4)
#define BTMW_TEST_LOG_MASK    (BTMW_TEST_LOG_LV_VBS|BTMW_TEST_LOG_LV_INF|BTMW_TEST_LOG_LV_DBG|BTMW_TEST_LOG_LV_WRN|BTMW_TEST_LOG_LV_ERR)

#define BTMW_TEST_LOG_FLAG_COLOR             (1 << 8)
#define BTMW_TEST_LOG_FLAG_TIMESTAMP         (1 << 9)

#define BTMW_TEST_Logv(fmt, ...) BTMW_TEST_Log(BTMW_TEST_LOG_LV_VBS, "<V> " fmt , ## __VA_ARGS__)
#define BTMW_TEST_Logi(fmt, ...) BTMW_TEST_Log(BTMW_TEST_LOG_LV_INF, "<I> " fmt , ## __VA_ARGS__)
#define BTMW_TEST_Logd(fmt, ...) BTMW_TEST_Log(BTMW_TEST_LOG_LV_DBG, "<D> " fmt , ## __VA_ARGS__)
#define BTMW_TEST_Logw(fmt, ...) BTMW_TEST_Log(BTMW_TEST_LOG_LV_WRN, "<W> %s(). " fmt , __func__, ## __VA_ARGS__)
#define BTMW_TEST_Loge(fmt, ...) BTMW_TEST_Log(BTMW_TEST_LOG_LV_ERR, "<E> %s()[%d] : " fmt , __func__, __LINE__, ## __VA_ARGS__)

void BTMW_TEST_Log(unsigned char lv, const char *fmt, ...);
void BTMW_TEST_Log_SetFlag(unsigned short flag);

#endif  //__BTMW_TEST_DEBUG_H__
