#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>

#include <ESPAsyncWebServer.h>

#include "ESPUIclientFsm.h"
#include "ESPUIcontrol.h"

class ESPUIclient
{
public:
    enum ClientUpdateType_t
    { // this is an orderd list. highest number is highest priority
        Synchronized    = 0,
        UpdateNeeded    = 1,
        RebuildNeeded   = 2,
        ReloadNeeded    = 3,
    };

protected:
    // bool HasBeenNotified      = false;  // Set when a notification has been sent and we are waiting for a reply
    // bool DelayedNotification  = false;  // set if a delayed notification is needed

    ClientUpdateType_t ClientUpdateType = ClientUpdateType_t::RebuildNeeded;

#if defined(ESP32) || defined(ESP8266)
    AsyncWebSocketClient * client = nullptr;
#else
    // RP2040/RP2350: No async WebSocket client
    void * client = nullptr;
#endif

    friend class fsm_EspuiClient_state_Idle;
    friend class fsm_EspuiClient_state_SendingUpdate;
    friend class fsm_EspuiClient_state_Rebuilding;
    friend class fsm_EspuiClient_state_WaitForAck;
    friend class fsm_EspuiClient_state_Reloading;
    friend class fsm_EspuiClient_state;

    fsm_EspuiClient_state_Idle          fsm_EspuiClient_state_Idle_imp;
    fsm_EspuiClient_state_SendingUpdate fsm_EspuiClient_state_SendingUpdate_imp;
    fsm_EspuiClient_state_Rebuilding    fsm_EspuiClient_state_Rebuilding_imp;
    fsm_EspuiClient_state_Reloading     fsm_EspuiClient_state_Reloading_imp;
    fsm_EspuiClient_state* pCurrentFsmState = &fsm_EspuiClient_state_Idle_imp;

    time_t      EspuiClientEndTime = 0;

    // bool        NeedsNotification() { return pCurrentFsmState != &fsm_EspuiClient_state_Idle_imp; }

    bool        CanSend();
    void        FillInHeader(ArduinoJson::JsonDocument& document);
    uint32_t    prepareJSONChunk(uint16_t startindex, JsonDocument& rootDoc, bool InUpdateMode, String value);
    bool        SendControlsToClient(uint16_t startidx, ClientUpdateType_t TransferMode, String FragmentRequest);

    bool        SendClientNotification(ClientUpdateType_t value);

private:
    uint32_t    CurrentSyncID = 0;
    uint32_t    NextSyncID = 0;

public:
#if defined(ESP32) || defined(ESP8266)
                ESPUIclient(AsyncWebSocketClient * _client);
#else
                ESPUIclient(void * _client = nullptr);
#endif
                ESPUIclient(const ESPUIclient & source);
    virtual     ~ESPUIclient();
    void        NotifyClient(ClientUpdateType_t value);
#if defined(ESP32) || defined(ESP8266)
    bool        onWsEvent(AwsEventType type, void* arg, uint8_t* data, size_t len);
#else
    bool        onWsEvent(int type, void* arg, uint8_t* data, size_t len);
#endif
    bool        IsSyncronized();
#if defined(ESP32) || defined(ESP8266)
    uint32_t    id() { return client->id(); }
#else
    uint32_t    id() { return static_cast<uint32_t>(reinterpret_cast<uintptr_t>(client)); }
#endif
    void        SetState(ClientUpdateType_t value);
    bool        SendJsonDocToWebSocket(ArduinoJson::JsonDocument& document);

};
