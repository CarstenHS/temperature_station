import argparse
from jinja2 import Template

# Define the Jinja2 template for the config.h file
config_h_template = '''
#ifndef CONFIG_H
#define CONFIG_H

// Configuration parameters
#define WIFI_SSID "{{ WIFI_SSID }}"
#define WIFI_PASS "{{ WIFI_PASS }}"
#define STORAGEACCOUNT_NAME "{{ STORAGEACCOUNT_NAME }}"
#define TABLE_NAME "{{ TABLE_NAME }}"
#define TENANT_ID "{{ TENANT_ID }}"
#define CLIENT_ID "{{ CLIENT_ID }}"
#define CLIENT_SECRET "{{ CLIENT_SECRET }}"
#define DEVICE_ID "{{ DEVICE_ID }}"
#define UPLOAD_TIMEOUT {{ UPLOAD_TIMEOUT }}

#endif // CONFIG_H
'''

def generate_config_h(WIFI_SSID, WIFI_PASS, STORAGEACCOUNT_NAME, TABLE_NAME, TENANT_ID, CLIENT_ID, CLIENT_SECRET, DEVICE_ID, UPLOAD_TIMEOUT):
    # Create a template from the string
    template = Template(config_h_template)

    # Render the template with provided parameters
    config_content = template.render(
        WIFI_SSID=WIFI_SSID,
        WIFI_PASS=WIFI_PASS,
        STORAGEACCOUNT_NAME=STORAGEACCOUNT_NAME,
        TABLE_NAME=TABLE_NAME,
        TENANT_ID=TENANT_ID,
        CLIENT_ID=CLIENT_ID,
        CLIENT_SECRET=CLIENT_SECRET,
        DEVICE_ID=DEVICE_ID,
        UPLOAD_TIMEOUT=UPLOAD_TIMEOUT
    )

    # Write the rendered content to a file
    with open("Config.h", 'w') as file:
        file.write(config_content)

    print(f'config.h file has been created at Config.h')

if __name__ == '__main__':
    # Create argument parser
    parser = argparse.ArgumentParser(description='Generate a config.h file with 9 parameters.')
    
    # Add arguments for the 9 parameters
    parser.add_argument('--WIFI_SSID', type=str, help='WiFi SSID (string)')
    parser.add_argument('--WIFI_PASS', type=str, help='WiFi password (string)')
    parser.add_argument('--STORAGEACCOUNT_NAME', type=str, help='Storage account name (string)')
    parser.add_argument('--TABLE_NAME', type=str, help='Table name (string)')
    parser.add_argument('--TENANT_ID', type=str, help='Tenant ID (string)')
    parser.add_argument('--CLIENT_ID', type=str, help='Client ID (string)')
    parser.add_argument('--CLIENT_SECRET', type=str, help='Client secret (string)')
    parser.add_argument('--DEVICE_ID', type=str, help='Device ID (string)')
    parser.add_argument('--UPLOAD_TIMEOUT', type=int, help='Upload timeout in seconds (integer)')
    
    # Parse the arguments from the command line
    args = parser.parse_args()

    # Call the function with command-line arguments
    generate_config_h(
        args.WIFI_SSID, args.WIFI_PASS, args.STORAGEACCOUNT_NAME, 
        args.TABLE_NAME, args.TENANT_ID, args.CLIENT_ID, 
        args.CLIENT_SECRET, args.DEVICE_ID, args.UPLOAD_TIMEOUT
    )
