#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "https_client.h"
#include "main.h"

#include "FreeRTOS.h"
#include "w6x_api.h"
#include "common_parser.h" /* Common Parser functions */
#include "shell.h"
#include "cJSON.h"
#include "w6x_types.h"


/* Private defines -----------------------------------------------------------*/
//#define WEATHER_SERVER_NAME "example.com"
//#define WEATHER_URL_REQUEST "https://" WEATHER_SERVER_NAME "/"

//#define WEATHER_SERVER_NAME "mikesse.pythonanywhere.com"
//#define WEATHER_URL_REQUEST "https://" WEATHER_SERVER_NAME "/debug/raw-body/"
//#define WEATHER_URL_REQUEST "https://" WEATHER_SERVER_NAME "/upload/"


//#define WEATHER_SERVER_NAME "www.st.com"
//#define WEATHER_URL_REQUEST "https://" WEATHER_SERVER_NAME "/content/st_com/en/about/"

#define WEATHER_SERVER_NAME "webhook.site"
#define WEATHER_URL_REQUEST "https://" WEATHER_SERVER_NAME "/e91143c5-758c-4e43-b8d9-b2b9afdace58?name=image1.png&deviceId=12345"

//#define WEATHER_SERVER_NAME "requestbin.net"
//#define WEATHER_URL_REQUEST "https://" WEATHER_SERVER_NAME "/bins/local/5a9583d7f6a99cd08962g1ati8hyyyyyb"

/** API Weather server url */
//#define WEATHER_SERVER_NAME "api.open-meteo.com"
//#define WEATHER_URL_REQUEST "https://" WEATHER_SERVER_NAME "/v1/forecast"

/* Private Varibles -----------------------------------------------------------*/

#if (LFS_ENABLE == 0)
/** Weather server certificate content */
/** * Combined Certificate Bundle
 * Includes ISRG Root X1, R12, and R13
 */
static char headerdata[10000];


/** ISRG Root X1 Certificate */
static const char isrgrootx1[] =
  "-----BEGIN CERTIFICATE-----\r\n"
  "MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw\r\n"
  "TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh\r\n"
  "cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4\r\n"
  "WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu\r\n"
  "ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY\r\n"
  "MTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0FDfzm54rVygc\r\n"
  "h77ct984kIxuPOZXoHj3dcKi/vVqbvYATyjb3miGbESTtrFj/RQSa78f0uoxmyF+\r\n"
  "0TM8ukj13Xnfs7j/EvEhmkvBioZxaUpmZmyPfjxwv60pIgbz5MDmgK7iS4+3mX6U\r\n"
  "A5/TR5d8mUgjU+g4rk8Kb4Mu0UlXjIB0ttov0DiNewNwIRt18jA8+o+u3dpjq+sW\r\n"
  "T8KOEUt+zwvo/7V3LvSye0rgTBIlDHCNAymg4VMk7BPZ7hm/ELNKjD+Jo2FR3qyH\r\n"
  "B5T0Y3HsLuJvW5iB4YlcNHlsdu87kGJ55tukmi8mxdAQ4Q7e2RCOFvu396j3x+UC\r\n"
  "B5iPNgiV5+I3lg02dZ77DnKxHZu8A/lJBdiB3QW0KtZB6awBdpUKD9jf1b0SHzUv\r\n"
  "KBds0pjBqAlkd25HN7rOrFleaJ1/ctaJxQZBKT5ZPt0m9STJEadao0xAH0ahmbWn\r\n"
  "OlFuhjuefXKnEgV4We0+UXgVCwOPjdAvBbI+e0ocS3MFEvzG6uBQE3xDk3SzynTn\r\n"
  "jh8BCNAw1FtxNrQHusEwMFxIt4I7mKZ9YIqioymCzLq9gwQbooMDQaHWBfEbwrbw\r\n"
  "qHyGO0aoSCqI3Haadr8faqU9GY/rOPNk3sgrDQoo//fb4hVC1CLQJ13hef4Y53CI\r\n"
  "rU7m2Ys6xt0nUW7/vGT1M0NPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNV\r\n"
  "HRMBAf8EBTADAQH/MB0GA1UdDgQWBBR5tFnme7bl5AFzgAiIyBpY9umbbjANBgkq\r\n"
  "hkiG9w0BAQsFAAOCAgEAVR9YqbyyqFDQDLHYGmkgJykIrGF1XIpu+ILlaS/V9lZL\r\n"
  "ubhzEFnTIZd+50xx+7LSYK05qAvqFyFWhfFQDlnrzuBZ6brJFe+GnY+EgPbk6ZGQ\r\n"
  "3BebYhtF8GaV0nxvwuo77x/Py9auJ/GpsMiu/X1+mvoiBOv/2X/qkSsisRcOj/KK\r\n"
  "NFtY2PwByVS5uCbMiogziUwthDyC3+6WVwW6LLv3xLfHTjuCvjHIInNzktHCgKQ5\r\n"
  "ORAzI4JMPJ+GslWYHb4phowim57iaztXOoJwTdwJx4nLCgdNbOhdjsnvzqvHu7Ur\r\n"
  "TkXWStAmzOVyyghqpZXjFaH3pO3JLF+l+/+sKAIuvtd7u+Nxe5AW0wdeRlN8NwdC\r\n"
  "jNPElpzVmbUq4JUagEiuTDkHzsxHpFKVK7q4+63SM1N95R1NbdWhscdCb+ZAJzVc\r\n"
  "oyi3B43njTOQ5yOf+1CceWxG1bQVs5ZufpsMljq4Ui0/1lvh+wjChP4kqKOJ2qxq\r\n"
  "4RgqsahDYVvTH9w7jXbyLeiNdd8XM2w9U/t7y0Ff/9yi0GE44Za4rF2LN9d11TPA\r\n"
  "mRGunUHBcnWEvgJBQl9nJEiU0Zsnvgc/ubhPgXRR4Xq37Z0j4r7g1SgEEzwxA57d\r\n"
  "emyPxgcYxn/eR44/KJ4EBs+lVDR3veyJm+kXQ99b21/+jh5Xos1AnX5iItreGCc=\r\n"
  "-----END CERTIFICATE-----\r\n";

#endif /* LFS_ENABLE */


/* Private Function Calls -----------------------------------------------------------*/

/**
  * @brief  Callback function to receive the data from the server
  * @param  arg: Callback argument
  * @param  p: Pointer to the data
  * @param  err: Error code
  * @return Operation status
  */
static int32_t HTTPS_recv_cb(void *arg, W6X_HTTP_buffer_t *p, int32_t err);


//Callback function to receive the header data from the server

static int32_t HTTPS_header_recv_cb(W6X_HTTP_state_t *connection,
									void *arg,
									uint8_t *hdr,
									uint16_t hdr_len,
									uint32_t content_len);

/* Client post function -----------------------------------------------------------*/

int32_t https_client_post(void){

	ip_addr_t addr = {0};
	W6X_HTTP_connection_t settings = {0};
	uint8_t https_server_addr[4] = {192, 168, 8, 1};

	char *method = "POST";
	char server_name[32] = WEATHER_SERVER_NAME;
	char certificate[16] = "isrgrootx1.pem";
	char url[256] = {0};
	char *token = NULL;
	char *address = NULL;

	/* Create the URL */
	//  snprintf(url, sizeof(url), WEATHER_URL_REQUEST, (double)city_latitude, (double)city_longitude, weather_metrics[0]);
	snprintf(url, sizeof(url), WEATHER_URL_REQUEST);


	token = strstr(url, "//");
	if ((token == NULL) || (strlen(token) < 3))
	{
	LogError("Mal-formed URL\n");
	return -1;
	}

	/* Get the Base URL */
	address = token + 2;
	char *uri = strstr(address, "/");
	if (uri == NULL)
	{
	LogError("Mal-formed URL\n");
	return -1;
	}

	/* Get the IP address from the Base URL */
	if (W6X_Net_Inet_pton(AF_INET, address, (const void *) &addr.u_addr.ip4.addr) != 0)
	{
		if (W6X_Net_ResolveHostAddress(server_name, https_server_addr) != W6X_STATUS_OK)
		{
			LogError("IP Address identification failed\n");
			return -1;
		}
		addr.u_addr.ip4.addr = ATON(https_server_addr);
	}


  /* Set the HTTPS settings */
    settings.server_name = server_name;
    settings.https_certificate.name = certificate;


	#if (LFS_ENABLE == 0)
	settings.https_certificate.content = (char *)isrgrootx1;
	#endif /* LFS_ENABLE */


	/* Register the callback function */
	settings.recv_fn = HTTPS_recv_cb;

//*************************************************************************************
//    W6X_FS_FilesListFull_t * files_list;
//    char *filename = "isrgrootx1.pem";
//    uint32_t offset = 0U;
//    uint32_t size;
//    volatile W6X_Status_t W6X_Status = W6X_FS_ListFiles(&files_list);
//    if (!W6X_Status){
//  	  W6X_Status = W6X_FS_GetSizeFile(filename, &size);
//    }
//    if (!W6X_Status){
//  	  W6X_Status = W6X_FS_ReadFile(filename, offset, (uint8_t *)weather_cert0, size);
//    }
//    if (!W6X_Status){
//  	  weather_cert0[size] = '\0';
//  	  LogInfo("%s", weather_cert0);
//  //	  while(1);
//    }
//*************************************************************************************
    char * data_content = "***This should be your Image Content***";
    W6X_HTTP_Post_Data_t post_data = {
        .data = data_content,
        .type = W6X_HTTP_CONTENT_TYPE_OCTET_STREAM
    };

	size_t post_data_len = strlen(post_data.data);
    /* Send the HTTP request. Non-blocking function. The response will be received by the callback */
    W6X_Status_t ret =  W6X_HTTP_Client_Request(&addr, 443, uri, method,
    											(void *)&post_data,
												post_data_len,
												NULL, NULL, HTTPS_header_recv_cb,
												NULL, &settings);
	//    W6X_Status_t ret =  W6X_HTTP_Client_Request(&addr, 443, uri, method, NULL, 0, NULL, NULL, NULL, NULL, &settings);
    if (ret != W6X_STATUS_OK)
    {
      return -1;
    }

    return 0;
}

static int32_t HTTPS_recv_cb(void *arg, W6X_HTTP_buffer_t *p, int32_t err){
	  int32_t ret = -1;

	  if (p->data == NULL || err)
	  {
		LogInfo("Possible Error : %d", err);
	    return ret;
	  }

//	  cJSON_Hooks hooks =
//	  {
//	    .malloc_fn = pvPortMalloc,
//	    .free_fn = vPortFree,
//	  };
//	  cJSON_InitHooks(&hooks);

		memcpy(&headerdata[2000], p->data, (size_t) p->length);
		headerdata[(uint16_t )(p->length+2000)] = '\0';
		LogInfo("\n\r*********************HTTPS response data*********************\n\r%s\n\r", &headerdata[2000]);

	  return 0;
}

static int32_t HTTPS_header_recv_cb(W6X_HTTP_state_t *connection,
									void *arg,
									uint8_t *hdr,
									uint16_t hdr_len,
									uint32_t content_len){
	(void) arg;
	(void) hdr;
	(void) hdr_len;
	(void) content_len;

	memcpy(headerdata, hdr, sizeof(headerdata)-1U);
	headerdata[hdr_len] = '\0';
	LogInfo("\n\r*********************HTTPS response header*********************\n\r%s\n\r", headerdata);
	  return 0;
}
