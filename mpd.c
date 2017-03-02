#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <regex.h>
#include <mpd/client.h>

#define MPDHOST "localhost"
#define MPDPORT 6600

enum {  MpdNext,
        MpdPrev,
        MpdStop,
        MpdToggle,
        MpdRaiseVolume,
        MpdLowerVolume };


struct mpd_connection *get_conn(){
  struct mpd_connection *c;

  c = mpd_connection_new(MPDHOST, MPDPORT, 1000);

  if(mpd_connection_get_error(c) != MPD_ERROR_SUCCESS){
    fprintf(stderr, "Could not connect to mpd: %s\n", mpd_connection_get_error_message(c));
    mpd_connection_free(c);
    return NULL;
  }

  return NULL;
}

void mpd_stop(struct mpd_connection *c) {
  struct mpd_status *s;

  if (c == NULL) {
    return;
  }

  s = mpd_run_status(c);
  if (s != NULL) {
    if ( mpd_status_get_state(s) == MPD_STATE_STOP) {
      return;
    }
    else{
      mpd_run_stop(c);
      return;
    }
  }
  else {
    fprintf(stderr, "Could not get mpd status: %s\n", mpd_status_get_error(s));
    mpd_status_free(s);
    mpd_connection_free(c);
    return;
  }
}

void mpd_next(struct mpd_connection *c) {
  if (c == NULL) {
    return;
  }
  else {
    mpd_run_next(c);
  }
  mpd_connection_free(c);
}

void mpd_prev(struct mpd_connection *c) {
  if (c == NULL) {
    return;
  }
  else {
    mpd_run_previous(c);
  }
  mpd_connection_free(c);
}

void mpd_toggle(struct mpd_connection *c) {
  struct mpd_status *s;
  struct mpd_song *song;
  const char *filename;
  enum mpd_state st;
  regex_t expr;

  if (c == NULL) {
    return;
  }

  s = mpd_run_status(c);
  if (s == NULL) {
    fprintf(stderr, "Could not get mpd status: %s\n", mpd_status_get_error(s));
    mpd_status_free(s);
    mpd_connection_free(c);
    return;
  }
  else {
    st = mpd_status_get_state(s);
    if(st == MPD_STATE_STOP || st == MPD_STATE_PAUSE){
      mpd_run_play(c);
      mpd_status_free(s);
      mpd_connection_free(c);
    }
    else{
      if(st != MPD_STATE_UNKNOWN) {
        song = mpd_run_current_song(c);
        if(song == NULL) {
          fprintf(stderr, "Error fetching current song!\n");
          mpd_song_free(song);
          mpd_status_free(s);
          mpd_connection_free(c);
          return;
        }
        filename = mpd_song_get_uri(song);
        int errcode = regcomp(&expr, "^[[:alnum:]]+://", REG_EXTENDED|REG_NOSUB);
        if(errcode != 0){
          mpd_song_free(song);
          mpd_status_free(s);
          mpd_connection_free(c);
          regfree(&expr);
          return;
        }
        int matchcode = regexec(&expr, filename, 0, NULL, 0);
        if(matchcode == 0){
          if(strstr(filename, "file://") == filename){
            mpd_run_toggle_pause(c);
          }
          else{
            mpd_run_stop(c);
          }
        }
        else if(matchcode == REG_NOMATCH){
            mpd_run_toggle_pause(c);
        }
        regfree(&expr);
        mpd_song_free(song);
        mpd_status_free(s);
        mpd_connection_free(c);
      }
    }
  }
}

void mpd_volume(struct mpd_connection *c, const Arg *arg){
  struct mpd_status *s;
  int vol = 0;

  if (c == NULL) {
    return;
  }

  s = mpd_run_status(c);
  if(s == NULL) return;
  vol = mpd_status_get_volume(s);

  switch(arg->i) {
    case MpdRaiseVolume:
      vol += voldelta;
      if(vol > 100) vol = 100;
      break;
    case MpdLowerVolume:
      vol -= voldelta;
      if(vol < 0) vol = 0;
      break;
  }
  mpd_run_set_volume(c, vol);
  mpd_status_free(s);
}

void mpd_control(const Arg *arg){
  struct mpd_connection *c;
  c = get_conn();
  switch(arg->i) {
    case MpdNext:
      mpd_next(c);
      break;
    case MpdPrev:
      mpd_prev(c);
      break;
    case MpdStop:
      mpd_stop(c);
      break;
    case MpdToggle:
      mpd_toggle(c);
      break;
    case MpdRaiseVolume:
    case MpdLowerVolume:
      mpd_volume(c, arg);
      break;
  }
}
