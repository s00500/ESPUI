#pragma once

#include <Arduino.h>
#include <ESPUI.h>
#include <ESPUIControl.h>

// implemented as a singleton
class _ESPUIcontrolMgr
{
public:
    _ESPUIcontrolMgr();
    ~_ESPUIcontrolMgr() {}
    _ESPUIcontrolMgr (_ESPUIcontrolMgr const&) = delete;
    void operator=(_ESPUIcontrolMgr const&)  = delete;

    Control::ControlId_t addControl(Control::Type type,
                                    const char* label,
                                    const String& value,
                                    Control::Color color,
                                    Control::ControlId_t parentControl,
                                    bool visible,
                                    std::function<void(Control*, int)> callback);
    bool removeControl(Control::ControlId_t id);
    void RemoveToBeDeletedControls();

    Control* getControl(Control::ControlId_t id);
    Control* getControlNoLock(Control::ControlId_t id);
    Control::ControlId_t GetControlCount() {return controlCount;}

    uint32_t prepareJSONChunk(uint16_t startindex,
                              JsonDocument & rootDoc,
                              bool InUpdateMode,
                              String FragmentRequestString,
                              uint32_t CurrentSyncID);
private:
    class ControlObject_t : public Control
    {
public:
        ControlObject_t(Control::ControlId_t id,
                        Type type,
                        const char* label,
                        std::function<void(Control*, int)> callback,
                        const String& value,
                        Color color,
                        bool visible,
                        uint16_t parentControl);

        ControlObject_t * next = nullptr;
    }; // ControlObject_t

    ControlObject_t * controls = nullptr;
    Control::ControlId_t controlCount = 0;
    Control::ControlId_t idCounter = 0;
    ControlObject_t * getControlObjectNoLock(Control::ControlId_t id);

#ifdef ESP32
    SemaphoreHandle_t ControlsSemaphore = NULL;
#endif // def ESP32

};

extern _ESPUIcontrolMgr ESPUIcontrolMgr;