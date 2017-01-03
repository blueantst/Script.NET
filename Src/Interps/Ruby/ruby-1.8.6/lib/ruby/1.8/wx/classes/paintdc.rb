# Device Context to paint within an on_paint handler
class Wx::PaintDC
  # This class should not be instantiated directly in wxRuby; it should
  # always be used via Window#paint, which takes a block receiving the
  # DC. This ensures that the DC is cleaned up at the correct time,
  # preventing serious errors on some platforms.
  define_method(:initialize) do | *args |
    Kernel.raise RuntimeError,
                 "Do not instantiate PaintDC directly; use Window#paint",
                 caller[1..-1]
  end
end
