# A ListBox, where each item has a checkbox next to it
class Wx::CheckListBox
  # wxRuby redefinitions of core methods to add item data support (not
  # available in wxWidgets). Use a private method to initialize and
  # access the data, rather than in initialize(), b/c can't guarantee
  # that initialize will be called (eg when loaded from XRC)
  def __wx_item_data
    @__wx_item_data ||= []
  end
  private :__wx_item_data

  # Call method in ControlWithItems, then sync item data
  def append(item, data = nil)
    super(item)
    __wx_item_data[count - 1] = data
  end

  # Call method in ControlWithItems, then sync item data
  def clear
    super
    __wx_item_data.clear
  end

  # delete is defined in Wx::CheckListBox, so must be called in this class
  wx_delete = self.instance_method(:delete)
  define_method(:delete) do | n |
    wx_delete.bind(self).call(n)
    __wx_item_data.delete_at(n)
  end

  # Returns an array of indices of items that are currently checked.
  def get_checked_items
    find_all { | i | checked?(i) }    
  end
  
  # Wholly redefined here
  def get_item_data(n)
    __wx_item_data[n]
  end

  # Wholly redefined here
  def set_item_data(n, data)
    __wx_item_data[n] = data
  end
end
