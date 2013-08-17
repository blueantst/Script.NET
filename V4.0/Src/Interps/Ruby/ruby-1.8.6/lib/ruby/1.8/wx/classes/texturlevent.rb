# Event fired when a user clicks or hovers over a hyperlink in a TextCtrl
class Wx::TextUrlEvent
  # Returns the string URL that is being interacted with in this event
  def get_url
    text_ctrl = event_object
    # In the standard TextCtrl, the relevant URL is always the same as
    # the text clicked on, because all it does is highlight text that
    # look like a URL. In RichTextCtrl, however, the URL value can be
    # applied to any text (more like HTML), and so the URL has be
    # retrieved by getting the applied RichTextAttr style and inspecting
    # its property.
    if text_ctrl.kind_of?(Wx::RichTextCtrl)
      return text_ctrl.get_style(url_start).url
    else      
      return text_ctrl.value[url_start .. url_end]
    end
  end
end
