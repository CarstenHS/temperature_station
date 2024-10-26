# Status

* Needs coding of app owner role.
* Table contributor for SP on table.

# How to create Config.h:
``` bash
python generate_config.py --WIFI_SSID "MyWiFiSSID" --WIFI_PASS "MyWiFiPassword" --STORAGEACCOUNT_NAME "MyStorageAccount" --TABLE_NAME "MyTable" --TENANT_ID "MyTenantID" --CLIENT_ID "MyClientID" --CLIENT_SECRET "MyClientSecret" --DEVICE_ID "MyDeviceID" --UPLOAD_TIMEOUT 300 --output my_config.h
```