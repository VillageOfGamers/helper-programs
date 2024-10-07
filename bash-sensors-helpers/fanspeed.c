#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cjson/cJSON.h"
#define BUFFER_SIZE 4096

typedef struct {
    char name[50];
    int fan_speed;
} FanSpeedEntry;

int main() {
    int current_fan_speed = -100;
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
    const char *fan_speed_unit = " RPM";
    cJSON *sensor = NULL;
    cJSON_ArrayForEach(sensor, json) {
        cJSON *core = NULL;
        cJSON_ArrayForEach(core, sensor) {
            cJSON *fan_speed_entry = cJSON_GetObjectItem(core, "fan1_input");
            if (cJSON_IsNumber(fan_speed_entry)) {
                current_fan_speed = fan_speed_entry->valueint;
            }
        }
    }
    if (current_fan_speed != -100) {
        printf("%i%s\n", current_fan_speed, fan_speed_unit);
    } else {
        printf("No fan speed data found.\n");
    }
    cJSON_Delete(json);
    return 0;
}