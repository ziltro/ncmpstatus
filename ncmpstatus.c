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
		int timeTotal, timeElapsed;
		timeTotal = mpd_status_get_total_time(status);
		timeElapsed = mpd_status_get_elapsed_time(status);

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

		printf("%i   -   %i   -   %i\n", timeElapsed, timeTotal, timeTotal - timeElapsed);
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
