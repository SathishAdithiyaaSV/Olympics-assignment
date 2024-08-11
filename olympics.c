#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VENUES 100
#define MAX_EVENTS 24
#define MAX_NAME_LENGTH 50
#define MAX_INPUT_LENGTH 200

typedef struct {
    char name[MAX_NAME_LENGTH];
    int startHour;
    int endHour;
} Event;

typedef struct {
    char name[MAX_NAME_LENGTH];
    char location[MAX_NAME_LENGTH];
    int capacity;
    Event *calendar[30][MAX_EVENTS];  // Assuming a month with 30 days and max 24 events per day
    int eventCount[30];
} Venue;

Venue *venues[MAX_VENUES];
int venueCount = 0;

void initializeVenue(Venue *venue, const char *name, const char *location, int capacity) {
    strcpy(venue->name, name);
    strcpy(venue->location, location);
    venue->capacity = capacity;
    for (int i = 0; i < 30; i++) {
        venue->eventCount[i] = 0;
        for (int j = 0; j < MAX_EVENTS; j++) {
            venue->calendar[i][j] = NULL;
        }
    }
}

int addVenue(const char *name, const char *location, int capacity) {
    printf("%s %s %d\n", name, location, capacity);
    if (venueCount >= MAX_VENUES) {
        printf("-1\nError: out of memory\n");
        return -1;
    }

    for (int i = 0; i < venueCount; i++) {
        if (strcmp(venues[i]->name, name) == 0) {
            printf("-1\nError: duplicate venue name\n");
            return -1;
        }
    }

    Venue *newVenue = (Venue *)malloc(sizeof(Venue));
    if (!newVenue) {
        printf("-1\nError: out of memory\n");
        return -1;
    }

    initializeVenue(newVenue, name, location, capacity);
    venues[venueCount++] = newVenue;
    printf("0\n");
    return 0;
}

int deleteVenue(const char *name) {
    for (int i = 0; i < venueCount; i++) {
        if (strcmp(venues[i]->name, name) == 0) {
            free(venues[i]);
            for (int j = i; j < venueCount - 1; j++) {
                venues[j] = venues[j + 1];
            }
            venueCount--;
            printf("0\n");
            return 0;
        }
    }
    printf("-1\nError: venue does not exist\n");
    return -1;
}

void showVenues() {
    printf("%d\n", venueCount);
    for (int i = 0; i < venueCount; i++) {
        printf("%s\n%s %d\n", venues[i]->name, venues[i]->location, venues[i]->capacity);
    }
}

int addEvent(const char *venueName, int date, int fromHour, int toHour, const char *eventName) {
    Venue *venue = NULL;
    for (int i = 0; i < venueCount; i++) {
        if (strcmp(venues[i]->name, venueName) == 0) {
            venue = venues[i];
            break;
        }
    }

    if (!venue) {
        printf("-1\nError: no such venue\n");
        return -1;
    }

    for (int i = 0; i < venue->eventCount[date - 1]; i++) {
        Event *event = venue->calendar[date - 1][i];
        if ((fromHour >= event->startHour && fromHour < event->endHour) || 
            (toHour > event->startHour && toHour <= event->endHour)) {
            printf("-1\nError: event overlap\n");
            return -1;
        }
    }

    if (venue->eventCount[date - 1] >= MAX_EVENTS) {
        printf("-1\nError: out of memory\n");
        return -1;
    }

    Event *newEvent = (Event *)malloc(sizeof(Event));
    if (!newEvent) {
        printf("-1\nError: out of memory\n");
        return -1;
    }

    strcpy(newEvent->name, eventName);
    newEvent->startHour = fromHour;
    newEvent->endHour = toHour;
    venue->calendar[date - 1][venue->eventCount[date - 1]++] = newEvent;
    printf("0\n");
    return 0;
}

int deleteEvent(const char *venueName, int date, int fromHour, const char *eventName) {
    Venue *venue = NULL;
    for (int i = 0; i < venueCount; i++) {
        if (strcmp(venues[i]->name, venueName) == 0) {
            venue = venues[i];
            break;
        }
    }

    if (!venue) {
        printf("-1\nError: no such venue\n");
        return -1;
    }

    for (int i = 0; i < venue->eventCount[date - 1]; i++) {
        Event *event = venue->calendar[date - 1][i];
        if (strcmp(event->name, eventName) == 0 && event->startHour == fromHour) {
            free(event);
            for (int j = i; j < venue->eventCount[date - 1] - 1; j++) {
                venue->calendar[date - 1][j] = venue->calendar[date - 1][j + 1];
            }
            venue->eventCount[date - 1]--;
            printf("0\n");
            return 0;
        }
    }

    printf("-1\nError: event does not exist\n");
    return -1;
}

void showEvents(const char *venueName, int date) {
    Venue *venue = NULL;
    for (int i = 0; i < venueCount; i++) {
        if (strcmp(venues[i]->name, venueName) == 0) {
            venue = venues[i];
            break;
        }
    }

    if (!venue) {
        printf("-1\nError: no such venue\n");
        return;
    }

    printf("%d\n", venue->eventCount[date - 1]);
    for (int i = 0; i < venue->eventCount[date - 1]; i++) {
        Event *event = venue->calendar[date - 1][i];
        printf("%s %d %d\n", event->name, event->startHour, event->endHour);
    }
}

void showCalendar(const char *venueName) {
    Venue *venue = NULL;
    for (int i = 0; i < venueCount; i++) {
        if (strcmp(venues[i]->name, venueName) == 0) {
            venue = venues[i];
            break;
        }
    }

    if (!venue) {
        printf("-1\nError: no such venue\n");
        return;
    }

    int totalEvents = 0;
    for (int i = 0; i < 30; i++) {
        totalEvents += venue->eventCount[i];
    }
    printf("%d\n", totalEvents);

    for (int i = 0; i < 30; i++) {
        printf("%d %d\n", i + 1, venue->eventCount[i]);
        for (int j = 0; j < venue->eventCount[i]; j++) {
            Event *event = venue->calendar[i][j];
            printf("%s %d %d\n", event->name, event->startHour, event->endHour);
        }
    }
}

int findChar(char string[], char ch)
{
    for(int i = 0;i < strlen(string);i++)
    {
        if(string[i] == ch)
        {
            return i;
        }
    }
    return -1;
}


int main() {
    char input[MAX_INPUT_LENGTH];
    char command[MAX_NAME_LENGTH];
    char venueName[MAX_NAME_LENGTH], location[MAX_NAME_LENGTH], eventName[MAX_NAME_LENGTH];
    int capacity, date, fromHour, toHour;
    char ch = '"';

    printf("Please enter a command...\n");
    while (1) {
        fgets(input, MAX_INPUT_LENGTH, stdin);
        input[strcspn(input, "\n")] = 0; // Remove newline character
        printf("%s\n", input);
        sscanf(input, "%s", command);
        printf("%s\n", input);
        if (strcmp(command, "addVenue") == 0) {
            strncpy(venueName, "", MAX_NAME_LENGTH);
            strncpy(location, "", MAX_NAME_LENGTH);
            int pos = findChar(input, ch);
            int pos2 = findChar(input+pos+1, ch);
            strncpy(venueName, input+pos+1, pos2);
            int a = findChar(input+pos+pos2+2, ch);
            int b = findChar(input+pos+pos2+a+3, ch);
            strncpy(location, input+pos+pos2+a+3, b);
            capacity = atoi(input+pos+pos2+a+b+5);
            addVenue(venueName, location, capacity);
        } else if (strcmp(command, "deleteVenue") == 0) {
            strncpy(venueName, "", MAX_NAME_LENGTH);
            int pos = findChar(input, ch);
            int pos2 = findChar(input+pos+1, ch);
            strncpy(venueName, input+pos+1, pos2);
            deleteVenue(venueName);
        } else if (strcmp(command, "showVenues") == 0) {
            showVenues();
        } else if (strcmp(command, "addEvent") == 0) {
            sscanf(input, "%*s \"%[^\"]\" %d %d %d \"%[^\"]\"", venueName, &date, &fromHour, &toHour, eventName);
            addEvent(venueName, date, fromHour, toHour, eventName);
        } else if (strcmp(command, "deleteEvent") == 0) {
            sscanf(input, "%*s \"%[^\"]\" %d %d \"%[^\"]\"", venueName, &date, &fromHour, eventName);
            deleteEvent(venueName, date, fromHour, eventName);
        } else if (strcmp(command, "showEvents") == 0) {
            sscanf(input, "%*s \"%[^\"]\" %d", venueName, &date);
            showEvents(venueName, date);
        } else if (strcmp(command, "showCalendar") == 0) {
            sscanf(input, "%*s \"%[^\"]\"", venueName);
            showCalendar(venueName);
        } else if (strcmp(command, "End") == 0) {
            break;
        } else {
            printf("-1\nError: invalid command\n");
        }
        printf("Please enter a command...\n");
    }

    // Free allocated memory
    for (int i = 0; i < venueCount; i++) {
        for (int j = 0; j < 30; j++) {
            for (int k = 0; k < venues[i]->eventCount[j]; k++) {
                free(venues[i]->calendar[j][k]);
            }
        }
        free(venues[i]);
    }

    return 0;
}
