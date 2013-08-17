class Wx::HtmlHelpController
  def self.instance(*args)
    @instance ||= new(*args)
  end
end
