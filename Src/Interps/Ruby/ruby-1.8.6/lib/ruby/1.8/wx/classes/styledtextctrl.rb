# Functionality here must be loaded first to add custom events
require 'wx/classes/evthandler'

# These event type constants will only be available and meaningful if
# Wx::StyledTextCtrl has been compiled into the library. If so, they
# need the below definitions for mapping, otherwise all the rest should
# be skipped
if defined?(Wx::StyledTextCtrl)
  EventType = Wx::EvtHandler::EventType

  stc_event_types = [ 
    EventType['evt_stc_calltip_click', 1,
              Wx::EVT_STC_CALLTIP_CLICK, 
              Wx::StyledTextEvent],
    EventType['evt_stc_change', 1,
              Wx::EVT_STC_CHANGE, 
              Wx::StyledTextEvent],
    EventType['evt_stc_charadded', 1,
              Wx::EVT_STC_CHARADDED, 
              Wx::StyledTextEvent],
    EventType['evt_stc_doubleclick', 1,
              Wx::EVT_STC_DOUBLECLICK, 
              Wx::StyledTextEvent],
    EventType['evt_stc_do_drop', 1,
              Wx::EVT_STC_DO_DROP,
              Wx::StyledTextEvent],
    EventType['evt_stc_drag_over', 1,
              Wx::EVT_STC_DRAG_OVER,
              Wx::StyledTextEvent],
    EventType['evt_stc_dwellend', 1,
              Wx::EVT_STC_DWELLEND,
              Wx::StyledTextEvent],
    EventType['evt_stc_dwellstart', 1,
              Wx::EVT_STC_DWELLSTART,
              Wx::StyledTextEvent],
    EventType['evt_stc_hotspot_click', 1,
              Wx::EVT_STC_HOTSPOT_CLICK,
              Wx::StyledTextEvent],
    EventType['evt_stc_hotspot_dclick', 1,
              Wx::EVT_STC_HOTSPOT_DCLICK,
              Wx::StyledTextEvent],
    EventType['evt_stc_key', 1,
              Wx::EVT_STC_KEY,
              Wx::StyledTextEvent],
    EventType['evt_stc_macrorecord', 1,
              Wx::EVT_STC_MACRORECORD, 
              Wx::StyledTextEvent],
    EventType['evt_stc_marginclick', 1,
              Wx::EVT_STC_MARGINCLICK, 
              Wx::StyledTextEvent],
    EventType['evt_stc_modified', 1,
              Wx::EVT_STC_MODIFIED, 
              Wx::StyledTextEvent],
    EventType['evt_stc_needshown', 1,
              Wx::EVT_STC_NEEDSHOWN, 
              Wx::StyledTextEvent],
    EventType['evt_stc_painted', 1,
              Wx::EVT_STC_PAINTED, 
              Wx::StyledTextEvent],
    EventType['evt_stc_romodifyattempt', 1,
              Wx::EVT_STC_ROMODIFYATTEMPT, 
              Wx::StyledTextEvent],
    EventType['evt_stc_savepointleft', 1,
              Wx::EVT_STC_SAVEPOINTLEFT, 
              Wx::StyledTextEvent],
    EventType['evt_stc_savepointreached', 1,
              Wx::EVT_STC_SAVEPOINTREACHED, 
              Wx::StyledTextEvent],
    EventType['evt_stc_start_drag', 1,
              Wx::EVT_STC_START_DRAG, 
              Wx::StyledTextEvent],
    EventType['evt_stc_styleneeded', 1,
              Wx::EVT_STC_STYLENEEDED, 
              Wx::StyledTextEvent],
    EventType['evt_stc_updateui', 1,
              Wx::EVT_STC_UPDATEUI, 
              Wx::StyledTextEvent],
    EventType['evt_stc_uridropped', 1,
              Wx::EVT_STC_URIDROPPED, 
              Wx::StyledTextEvent],
    EventType['evt_stc_userlistselection', 1,
              Wx::EVT_STC_USERLISTSELECTION, 
              Wx::StyledTextEvent],
    EventType['evt_stc_zoom', 1,
              Wx::EVT_STC_ZOOM, 
              Wx::StyledTextEvent]
    ]

  stc_event_types.each do | ev_type | 
    Wx::EvtHandler.register_event_type(ev_type)
  end
end
