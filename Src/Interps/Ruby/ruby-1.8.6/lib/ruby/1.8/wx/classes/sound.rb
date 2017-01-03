# Plays simple sounds from .wav files
class Wx::Sound
  class << self
    # Shortcut class method, as per the wxWidgets docs, but easier to do
    # in Ruby
    def play(file, flags = Wx::SOUND_ASYNC)
      new(file).play(flags)
    end
  end

  # Test if newly created Sound is valid; if not, raise an exception
  # rather than failing silently.
  wx_init = self.instance_method(:initialize)
  define_method(:initialize) do | *args |
    if not File.exist?( File.expand_path(args[0]) )
      Kernel.raise(ArgumentError, "Sound file does not exist: #{args[0]}")
    end
    wx_init.bind(self).call(*args)
    if not ok?
      Kernel.raise(ArgumentError, "Error loading sound file #{args[0]}")
    end
  end
end
