#include "ESPUI.h"
#include "ESPUIclient.h"

//----------------------------------------------
// FSM definitions
//----------------------------------------------
void fsm_EspuiClient_state::Init()
{
    // Serial.println(String("fsm_EspuiClient_state:Init: ") + GetStateName());
    Parent->pCurrentFsmState = this;
}

//----------------------------------------------
//----------------------------------------------
//----------------------------------------------
bool fsm_EspuiClient_state_Idle::NotifyClient()
{
    bool Response = false;

    // Serial.println(F("fsm_EspuiClient_state_Idle: NotifyClient"));
    ClientUpdateType_t TypeToProcess = Parent->ClientUpdateType;
    // Clear the type so that we capture any changes in type that happen
    // while we are processing the current request.
    Parent->ClientUpdateType = ClientUpdateType_t::Synchronized;

    // Start processing the current request.
    switch (TypeToProcess)
    {
        case ClientUpdateType_t::Synchronized:
        {
            // Serial.println(F("fsm_EspuiClient_state_Idle: NotifyClient:State:Synchronized"));
            // Parent->fsm_EspuiClient_state_Idle_imp.Init();
            Response = true; // Parent->SendClientNotification(ClientUpdateType_t::UpdateNeeded);
            break;
        }
        case ClientUpdateType_t::UpdateNeeded:
        {
            // Serial.println(F("fsm_EspuiClient_state_Idle: NotifyClient:State:UpdateNeeded"));
            Parent->fsm_EspuiClient_state_SendingUpdate_imp.Init();
            Response = Parent->SendClientNotification(ClientUpdateType_t::UpdateNeeded);
            break;
        }
        case ClientUpdateType_t::RebuildNeeded:
        {
            // Serial.println(F("fsm_EspuiClient_state_Idle: NotifyClient:State:RebuildNeeded"));
            Parent->fsm_EspuiClient_state_Rebuilding_imp.Init();
            Response = Parent->SendClientNotification(ClientUpdateType_t::RebuildNeeded);
            break;
        }
        case ClientUpdateType_t::ReloadNeeded:
        {
            // Serial.println(F("fsm_EspuiClient_state_Idle: NotifyClient:State:ReloadNeeded"));
            Parent->fsm_EspuiClient_state_Reloading_imp.Init();
            Response = Parent->SendClientNotification(ClientUpdateType_t::ReloadNeeded);
            break;
        }
    }
    return Response;
}

void fsm_EspuiClient_state_Idle::ProcessAck(uint16_t ControlIndex, String FragmentRequestString)
{
    if(!emptyString.equals(FragmentRequestString))
    {
        // Serial.println(F("fsm_EspuiClient_state_Idle::ProcessAck:Fragmentation:Got fragment Header"));
        Parent->SendControlsToClient(ControlIndex, ClientUpdateType_t::UpdateNeeded, FragmentRequestString);
    }
    else
    {
        // This is an unexpected request for control data from the browser
        // treat it as if it was a rebuild operation
        // Serial.println(F("fsm_EspuiClient_state_Idle: ProcessAck:Error: Rebuild"));
        Parent->NotifyClient(ClientUpdateType_t::RebuildNeeded);
    }
}

//----------------------------------------------
//----------------------------------------------
//----------------------------------------------
bool fsm_EspuiClient_state_SendingUpdate::NotifyClient()
{
    // Serial.println(F("fsm_EspuiClient_state_SendingUpdate:NotifyClient"));
    return true; /* Ignore request */
}

void fsm_EspuiClient_state_SendingUpdate::ProcessAck(uint16_t ControlIndex, String FragmentRequest)
{
    // Serial.println(F("fsm_EspuiClient_state_SendingUpdate: ProcessAck"));
    if(Parent->SendControlsToClient(ControlIndex, ClientUpdateType_t::UpdateNeeded, FragmentRequest))
    {
        // No more data to send. Go back to idle or start next request
        Parent->fsm_EspuiClient_state_Idle_imp.Init();
        Parent->fsm_EspuiClient_state_Idle_imp.NotifyClient();
    }
}

//----------------------------------------------
//----------------------------------------------
//----------------------------------------------
void fsm_EspuiClient_state_Rebuilding::Init()
{
    // Serial.println(String("fsm_EspuiClient_state:Init: ") + GetStateName());
    Parent->CurrentSyncID = 0;
    Parent->NextSyncID = 0;
    Parent->pCurrentFsmState = this;
}

bool fsm_EspuiClient_state_Rebuilding::NotifyClient()
{
    // Serial.println(F("fsm_EspuiClient_state_Rebuilding: NotifyClient"));
    return true; /* Ignore request */
}

void fsm_EspuiClient_state_Rebuilding::ProcessAck(uint16_t ControlIndex, String FragmentRequest)
{
    // Serial.println(F("fsm_EspuiClient_state_Rebuilding: ProcessAck"));
    if(Parent->SendControlsToClient(ControlIndex, ClientUpdateType_t::RebuildNeeded, FragmentRequest))
    {
        // No more data to send. Go back to idle or start next request
        Parent->fsm_EspuiClient_state_Idle_imp.Init();
        Parent->fsm_EspuiClient_state_Idle_imp.NotifyClient();
    }
}

//----------------------------------------------
//----------------------------------------------
//----------------------------------------------
void fsm_EspuiClient_state_Reloading::Init()
{
    // Serial.println(String("fsm_EspuiClient_state:Init: ") + GetStateName());
    Parent->CurrentSyncID = 0;
    Parent->NextSyncID = 0;
    Parent->pCurrentFsmState = this;
}

void fsm_EspuiClient_state_Reloading::ProcessAck(uint16_t ControlIndex, String FragmentRequestString)
{
    if(!emptyString.equals(FragmentRequestString))
    {
        // Serial.println(F("fsm_EspuiClient_state_Reloading::ProcessAck:Fragmentation:Got fragment Header"));
        Parent->SendControlsToClient(ControlIndex, ClientUpdateType_t::UpdateNeeded, FragmentRequestString);
    }
}

bool fsm_EspuiClient_state_Reloading::NotifyClient()
{
    // Serial.println(F("fsm_EspuiClient_state_Reloading: NotifyClient"));
    return true; /* Ignore request */
}
