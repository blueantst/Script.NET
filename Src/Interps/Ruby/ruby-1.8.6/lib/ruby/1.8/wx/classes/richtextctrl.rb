# Text editor supporting various formatting, intended for editing
# natural language texts
class Wx::RichTextCtrl
  # These three methods return the styles applicable at certain points
  # in the document. However, the standard wx signature is to accept the
  # value of some kind of TextAttr class that will hold the return as an
  # argument. The return value is a boolean for success/failure. In
  # Ruby, we only support returning the value as a RichTextAttr, and do
  # so as a proper return value, having accepted a single argument
  # specifying where to get the style from. If retrieval is not
  # successful, raise an exception.
  wx_get_style = self.instance_method(:get_style)
  define_method(:get_style) do | pos |
    style = Wx::RichTextAttr.new
    if wx_get_style.bind(self).call(pos, style)
      return style
    else
      Kernel.raise RuntimeError, "Could not retrieve style at position #{pos}"
    end
  end

  wx_get_style_for_range = self.instance_method(:get_style_for_range)
  define_method(:get_style_for_range) do | rng |
    style = Wx::RichTextAttr.new
    if wx_get_style_for_range.bind(self).call(rng, style)
      return style
    else
      Kernel.raise RuntimeError, "Could not retrieve style for range #{rng}"
    end
  end

  wx_get_uncombined_style = self.instance_method(:get_uncombined_style)
  define_method(:get_uncombined_style) do | pos |
    style = Wx::RichTextAttr.new
    if wx_get_uncombined_style.bind(self).call(pos, style)
      return style
    else
      Kernel.raise RuntimeError, "Could not retrieve style at position #{pos}"
    end
  end
end
