# Functionality here must be loaded first to add custom events
require 'wx/classes/evthandler'

# These event handlers should only be defined if MediaCtrl is actually
# available; some builds may not include it
if defined?(Wx::MediaCtrl)
  evt_type = Wx::EvtHandler::EventType
  mediactrl_event_types = [
    evt_type['evt_media_finished', 1,
              Wx::EVT_MEDIA_FINISHED,
              Wx::MediaEvent],
    evt_type['evt_media_loaded', 1,
              Wx::EVT_MEDIA_LOADED,
              Wx::MediaEvent],
    evt_type['evt_media_pause', 1,
              Wx::EVT_MEDIA_PAUSE,
              Wx::MediaEvent],
    evt_type['evt_media_play', 1,
              Wx::EVT_MEDIA_PLAY,
              Wx::MediaEvent],
    evt_type['evt_media_statechanged', 1,
              Wx::EVT_MEDIA_STATECHANGED,
              Wx::MediaEvent],
    evt_type['evt_media_stop', 1,
              Wx::EVT_MEDIA_STOP,
              Wx::MediaEvent]
  ]
  mediactrl_event_types.each do | ev_type | 
    Wx::EvtHandler.register_event_type(ev_type)
  end

  # Extend with a nicer interface to get_state
  class Wx::MediaCtrl
    # Returns true if the media is currently paused, else false
    def is_paused
      get_state == Wx::MEDIASTATE_PAUSED
    end
    # Returns true if the media is currently playing, else false
    def is_playing
      get_state == Wx::MEDIASTATE_PLAYING
    end
    # Returns true if the media is currently stopped, else false
    def is_stopped
      get_state == Wx::MEDIASTATE_STOPPED
    end
  end
end

