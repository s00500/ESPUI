#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>

// forward declaration
class ESPUIclient;

/*****************************************************************************/
/*
*	Generic fsm base class.
*/
/*****************************************************************************/
/*****************************************************************************/
class fsm_EspuiClient_state
{
public:
                 fsm_EspuiClient_state() {};
    virtual     ~fsm_EspuiClient_state() {}

            void Init();
    virtual bool NotifyClient() = 0;
    virtual void ProcessAck(uint16_t id) = 0;
    virtual String GetStateName () = 0;
            void SetParent(ESPUIclient * value) { Parent = value; }

protected:
    ESPUIclient * Parent = nullptr;

}; // fsm_EspuiClient_state

class fsm_EspuiClient_state_Idle : public fsm_EspuiClient_state
{
public:
                 fsm_EspuiClient_state_Idle() {}
    virtual     ~fsm_EspuiClient_state_Idle() {}

    virtual bool NotifyClient();
    virtual void ProcessAck(uint16_t id);
            String GetStateName() { return String(F("Idle")); }

}; // fsm_EspuiClient_state_Idle

class fsm_EspuiClient_state_SendingUpdate : public fsm_EspuiClient_state
{
public:
                 fsm_EspuiClient_state_SendingUpdate() {}
    virtual     ~fsm_EspuiClient_state_SendingUpdate() {}

    virtual bool NotifyClient();
    virtual void ProcessAck(uint16_t id);
            String GetStateName() { return String(F("Sending Update")); }

}; // fsm_EspuiClient_state_SendingUpdate

class fsm_EspuiClient_state_Rebuilding : public fsm_EspuiClient_state
{
public:
                 fsm_EspuiClient_state_Rebuilding() {}
    virtual     ~fsm_EspuiClient_state_Rebuilding() {}

    virtual bool NotifyClient();
    virtual void ProcessAck(uint16_t id);
            String GetStateName() { return String(F("Sending Rebuild")); }

}; // fsm_EspuiClient_state_Rebuilding

class fsm_EspuiClient_state_Reloading : public fsm_EspuiClient_state
{
public:
                 fsm_EspuiClient_state_Reloading() {}
    virtual     ~fsm_EspuiClient_state_Reloading() {}

    virtual bool NotifyClient() { return false; }
    virtual void ProcessAck(uint16_t) {}
            String GetStateName() { return String(F("Reloading")); }

}; // fsm_EspuiClient_state_Reloading

