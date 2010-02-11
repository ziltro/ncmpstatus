/* ncmpstatus
	Header, copyright, etc.
*/

#include <mpd/client.h>
#include <mpd/status.h>
#include <mpd/entity.h>
#include <mpd/search.h>
#include <mpd/tag.h>

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include <time.h>

#define SECS_PER_MINUTE 60
#define SECS_PER_HOUR 3600

int main(int argc, char ** argv) {
	struct mpd_connection *conn;

	conn = mpd_connection_new(NULL, 0, 30000);

	if (mpd_connection_get_error(conn) != MPD_ERROR_SUCCESS) {
		fprintf(stderr,"%s\n", mpd_connection_get_error_message(conn));
		mpd_connection_free(conn);
		return -1;
	}

	struct mpd_status * status;
	struct mpd_song *song;

	mpd_command_list_begin(conn, true);
	mpd_send_status(conn);
	mpd_send_current_song(conn);
	mpd_command_list_end(conn);

	status = mpd_recv_status(conn);
	if (status == NULL) {
		fprintf(stderr,"%s\n", mpd_connection_get_error_message(conn));
		mpd_connection_free(conn);
		return -1;
	}

	if (mpd_status_get_state(status) == MPD_STATE_PLAY ||
	    mpd_status_get_state(status) == MPD_STATE_PAUSE) {
		    
		if (mpd_status_get_state(status) == MPD_STATE_PLAY)
		{
			printf("Playing\n");
		}
		if (mpd_status_get_state(status) == MPD_STATE_PAUSE)
		{
			printf("Paused\n");
		}
		int timeTotal, timeElapsed, timeRemaining;
		timeTotal = mpd_status_get_total_time(status);
		timeElapsed = mpd_status_get_elapsed_time(status);
		timeRemaining = timeTotal - timeElapsed;

		mpd_response_next(conn);

		const char *artist;
		const char *title;

		printf("\n");

		while ((song = mpd_recv_song(conn)) != NULL) {
			artist = mpd_song_get_tag(song, MPD_TAG_ARTIST, 0);
			if (artist != NULL)
				printf("%s\n", artist);

			title = mpd_song_get_tag(song, MPD_TAG_TITLE, 0);
			if (title != NULL)
				printf("%s\n", title);

			if (artist == NULL && title == NULL)
				printf("%s\n", mpd_song_get_uri(song));

			mpd_song_free(song);
		}

		printf("\n");
		
		char sTimeElapsed[5];
		char sTimeTotal[5];
		char sTimeRemaining[5];

		if (timeTotal >= SECS_PER_HOUR)
		{
			sprintf(sTimeElapsed, "%02i:%02i:%02i", timeElapsed / SECS_PER_HOUR, (timeElapsed % SECS_PER_HOUR) / SECS_PER_MINUTE, timeElapsed % SECS_PER_MINUTE);
			sprintf(sTimeTotal, "%02i:%02i:%02i", timeTotal / SECS_PER_HOUR, (timeTotal % SECS_PER_HOUR) / SECS_PER_MINUTE, timeTotal % SECS_PER_MINUTE);
			sprintf(sTimeRemaining, "%02i:%02i:%02i", timeRemaining / SECS_PER_HOUR, (timeRemaining % SECS_PER_HOUR) / SECS_PER_MINUTE, timeRemaining % SECS_PER_MINUTE);
			printf("Elapsed      Duration     Remaining\n");
			printf("%s     %s     %s\n", sTimeElapsed, sTimeTotal, sTimeRemaining);
		}
		else
		{
			sprintf(sTimeElapsed, "%02i:%02i", timeElapsed / SECS_PER_MINUTE, timeElapsed % SECS_PER_MINUTE);
			sprintf(sTimeTotal, "%02i:%02i", timeTotal / SECS_PER_MINUTE, timeTotal % SECS_PER_MINUTE);
			sprintf(sTimeRemaining, "%02i:%02i", timeRemaining / SECS_PER_MINUTE, timeRemaining % SECS_PER_MINUTE);
			printf("Elapsed   Duration  Remaining\n");
			printf("%s     %s     %s\n", sTimeElapsed, sTimeTotal, sTimeRemaining);
		}
		
	}
	else {
		printf("Stopped\n");
	}

	if (mpd_connection_get_error(conn) != MPD_ERROR_SUCCESS) {
		fprintf(stderr,"%s\n", mpd_connection_get_error_message(conn));
		mpd_connection_free(conn);
		return -1;
	}

	if (mpd_connection_get_error(conn) != MPD_ERROR_SUCCESS) {
		fprintf(stderr,"%s\n", mpd_connection_get_error_message(conn));
		mpd_connection_free(conn);
		return -1;
	}

	if (!mpd_response_finish(conn)) {
		fprintf(stderr,"%s\n", mpd_connection_get_error_message(conn));
		mpd_connection_free(conn);
		return -1;
	}

	mpd_status_free(status);

	mpd_connection_free(conn);

	return 0;
}
