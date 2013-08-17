class Wx::XmlResource
  # XRC_NO_SUBCLASSING should always be in place in wxRuby - we can't
  # currently link directly to wxRuby subclasses.
  class << self
    wx_get = self.instance_method(:get)
    define_method(:get) do 
      res = wx_get.bind(self).call
      res.flags |= Wx::XRC_NO_SUBCLASSING
      res
    end
  end

  # WxRuby already has all XRC handlers built in so there's no way to
  # control init_all_handlers to reduce binary size. So save users
  # having to call it.
  wx_init = self.instance_method(:initialize)
  define_method(:initialize) do | *args |
    result = wx_init.bind(self).call(*args)
    result.flags |= Wx::XRC_NO_SUBCLASSING
    result.init_all_handlers
  end

  # The standard .load method returns a boolean indicating success or
  # failure. Failure might result from bad XML, or a non-existent
  # file. In ruby, in these circumstances, it's more natural to raise an
  # Exception than expect the user to test the return value.
  wx_load = self.instance_method(:load)
  define_method(:load) do | fname |
    result = wx_load.bind(self).call(fname)
    if not result
      Kernel.raise( RuntimeError,
                    "Failed to load XRC from '#{fname}'; " +
                    "check the file exists and is valid XML")
    end
    fname
  end
end
