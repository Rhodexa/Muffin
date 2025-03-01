#include "channel.hpp"

void Channel::setNoteOn(Channel& channel, bool value){
  channel.is_active = value;
}