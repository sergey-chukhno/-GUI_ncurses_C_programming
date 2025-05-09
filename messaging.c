#include "my_dispute.h"
#include "network.h"

int send_message(AppState *state, char *text)
{
  // Local handling (record message in local state)
  int channel_index = state->current_channel_index;
  Channel *channel = &state->channels[channel_index];

  // Check if we have space
  if (channel->message_count >= MAX_MESSAGES)
  {
    // Shift messages to make room
    for (int i = 0; i < MAX_MESSAGES - 1; i++)
    {
      memcpy(&channel->messages[i], &channel->messages[i + 1], sizeof(Message));
    }
    channel->message_count = MAX_MESSAGES - 1;
  }

  // Create new message
  Message *message = &channel->messages[channel->message_count];
  strncpy(message->text, text, MAX_MESSAGE_LEN - 1);
  strncpy(message->sender, state->users[state->current_user_index].username, MAX_USERNAME_LEN - 1);
  message->timestamp = time(NULL);

  // Update local state
  channel->message_count++;

  // Send message to server (channel_index + 1 for 1-based indexing on server)
  int result = network_send_channel_message(channel_index + 1, text);
  if (result)
  {
    fprintf(stderr, "Message sent to server successfully\n");
  }
  else
  {
    fprintf(stderr, "Warning: Failed to send message to server. Message saved locally only.\n");
  }

  return 1;
}

int send_private_message(AppState *state, char *username, char *text)
{
  // Find the user index by username
  int target_user_idx = -1;
  for (int i = 0; i < state->user_count; i++)
  {
    if (strcmp(state->users[i].username, username) == 0)
    {
      target_user_idx = i;
      break;
    }
  }

  if (target_user_idx == -1)
  {
    // User not found
    return 0;
  }

  // Create or find a DM channel for this user
  char dm_channel_name[MAX_CHANNEL_NAME_LEN];
  snprintf(dm_channel_name, MAX_CHANNEL_NAME_LEN, "DM-%s", username);

  int dm_channel_idx = -1;
  for (int i = 0; i < state->channel_count; i++)
  {
    if (strcmp(state->channels[i].name, dm_channel_name) == 0)
    {
      dm_channel_idx = i;
      break;
    }
  }

  // Create new DM channel if it doesn't exist
  if (dm_channel_idx == -1)
  {
    if (state->channel_count >= MAX_CHANNELS)
    {
      // No room for a new channel
      return 0;
    }

    dm_channel_idx = state->channel_count;
    strncpy(state->channels[dm_channel_idx].name, dm_channel_name, MAX_CHANNEL_NAME_LEN - 1);
    state->channels[dm_channel_idx].message_count = 0;
    state->channel_count++;
  }

  // Add message to the DM channel
  Channel *channel = &state->channels[dm_channel_idx];
  if (channel->message_count >= MAX_MESSAGES)
  {
    // Shift messages to make room
    for (int i = 0; i < MAX_MESSAGES - 1; i++)
    {
      memcpy(&channel->messages[i], &channel->messages[i + 1], sizeof(Message));
    }
    channel->message_count = MAX_MESSAGES - 1;
  }

  // Create new message
  Message *message = &channel->messages[channel->message_count];
  strncpy(message->text, text, MAX_MESSAGE_LEN - 1);
  strncpy(message->sender, state->users[state->current_user_index].username, MAX_USERNAME_LEN - 1);
  message->timestamp = time(NULL);

  // Update local state
  channel->message_count++;

  // Send message to server
  int result = network_send_private_message(username, text);
  if (result)
  {
    fprintf(stderr, "Private message sent to server successfully\n");
  }
  else
  {
    fprintf(stderr, "Warning: Failed to send private message to server. Message saved locally only.\n");
  }

  return 1;
}

int add_reaction(AppState *state, int message_index, char reaction)
{
  // Get the current channel
  Channel *channel = &state->channels[state->current_channel_index];

  // Check if message index is valid
  if (message_index < 0 || message_index >= channel->message_count)
  {
    return 0;
  }

  // Get the message
  Message *msg = &channel->messages[message_index];

  // Check if reaction already exists for this message
  for (int i = 0; i < MAX_REACTIONS; i++)
  {
    if (msg->reactions[i] == reaction)
    {
      // Increment the count for this reaction
      msg->reaction_count[i]++;
      return 1;
    }
    else if (msg->reactions[i] == 0)
    {
      // Found an empty reaction slot, add the new reaction
      msg->reactions[i] = reaction;
      msg->reaction_count[i] = 1;
      return 1;
    }
  }

  // If we get here, all reaction slots are full
  return 0;
}