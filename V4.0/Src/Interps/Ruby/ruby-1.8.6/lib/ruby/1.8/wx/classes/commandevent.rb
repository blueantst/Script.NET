# Class representing interactions with controls such as ListBox
class Wx::CommandEvent
  # get_int and get_selection are already synonyms, but neither name
  # accurately describes what the method does as the event may be a
  # (de)selection or a check in a CheckListBox
  alias :get_index :get_int
end
