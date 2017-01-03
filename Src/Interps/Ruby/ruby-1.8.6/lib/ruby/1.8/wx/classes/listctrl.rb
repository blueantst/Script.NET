# Multi-item control with numerous possible view styles
class Wx::ListCtrl
  # Make these ruby enumerables so find, find_all, map are available 
  include Enumerable
  # Passes each valid item index into the passed block
  def each
    0.upto(item_count - 1) { | i | yield i }
  end

  # Stub version for LC_VIRTUAL controls that does nothing; may be
  # overridden in subclasses.
  def on_get_item_attr(i)
    nil
  end

  # Stub version for LC_VIRTUAL|LC_REPORT controls that does nothing;
  # may be overridden in subclasses.
  def on_get_item_column_image(i, col)
    -1
  end
end
