#!/usr/bin/env bash
curl -v -X GET http://xxxxxxxx:8080/api/plugins/telemetry/DEVICE/9ca6cc60-26c2-11eb-b550-85ae845aa8d1/keys/timeseries -o telemetry_keys.json \
--header "Content-Type:application/json" \
--header "X-Authorization: Bearer eyJhbGciOiJIUzUxMiJ9.eyJzdWIiOiJlcndpbkBlcHRlY29uLm