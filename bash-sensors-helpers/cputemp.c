#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cjson/cJSON.h"
#define BUFFER_SIZE 4096

typedef struct {
    char name[50];
    int temperature;
} TempEntry;

int main() {
    int current_temp;
    char buffer[BUFFER_SIZE];
    FILE *fp;
    fp = popen("sensors -j", "r");
    if (fp == NULL) {
        printf("Failed to run sensors command\n");
        return 1;
    }
    size_t bytes_read = fread(buffer, 1, sizeof(buffer) - 1, fp);
    if (bytes_read == 0) {
        printf("No data read from sensors\n");
        pclose(fp);
        return 1;
    }
    buffer[bytes_read] = '\0';
    pclose(fp);
    cJSON *json = cJSON_Parse(buffer);
    if (json == NULL) {
        printf("Error parsing JSON\n");
        return 1;
    }
    const char *temp_unit = "°C";
    cJSON *sensor = NULL;
    cJSON_ArrayForEach(sensor, json) {
        cJSON *core = NULL;
        cJSON_ArrayForEach(core, sensor) {
            cJSON *temp_entry = cJSON_GetObjectItem(core, "temp1_input");
            if (cJSON_IsNumber(temp_entry)) {
                current_temp = temp_entry->valueint;
            }
        }
    }
    if (current_temp != 0) {
        printf("%i%s\n", current_temp, temp_unit);
    } else {
        printf("No temperature data found.\n");
    }
    cJSON_Delete(json);
    return 0;
}