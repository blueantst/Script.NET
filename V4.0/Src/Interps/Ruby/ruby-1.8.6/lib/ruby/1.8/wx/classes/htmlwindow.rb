class Wx::HtmlWindow
  # imitate the in-built LoadFile method
  def load_file(file)
    set_page( File.read(file) )
  end
  
  # This is called from within wxWidgets whenever a URL is being
  # opened. The method may return true, to signal that the opening
  # should be permitted, false or nil to signal that it should be
  # blocked, or a string URL to which it should be redirected
  def on_opening_url(url)
    true
  end
end
