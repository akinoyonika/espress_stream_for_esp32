#include <esp_random.h>
#include <WiFi.h>

#include <espress_stream_for_esp32.h>

#include "wifi_config.h"

extern "C" int exec_cipher_ed25519_auth(
  int (*cipher_ed25519_auth)(unsigned char *signature, unsigned char const *public_key, unsigned char const *message, unsigned message_size),
  unsigned char *signature,
  unsigned char const *public_key,
  unsigned char const *message,
  unsigned message_size
)
{
  struct param {
    int (*cipher_ed25519_auth)(unsigned char *signature, unsigned char const *public_key, unsigned char const *message, unsigned message_size);
    unsigned char *signature;
    unsigned char const *public_key;
    unsigned char const *message;
    unsigned message_size;
    int ret;
  } param = {
    cipher_ed25519_auth,
    signature,
    public_key,
    message,
    message_size,
    0
  };
  auto task = [](void *p) {
    ((struct param *)p)->ret = ((struct param *)p)->cipher_ed25519_auth(
      ((struct param *)p)->signature,
      ((struct param *)p)->public_key,
      ((struct param *)p)->message,
      ((struct param *)p)->message_size
    );
    vTaskDelete(NULL);
  };
  TaskHandle_t task_handle;
  TaskStatus_t task_status;
  xTaskCreate(task, "cipher_ed25519_auth", 16 * 1024, &param, 1, &task_handle);
  do {
    yield();
    vTaskGetInfo(task_handle, &task_status, pdFALSE, eInvalid);
  } while (task_status.eCurrentState != eDeleted);
  return param.ret;
}

extern "C" {
extern void es_ccat_main(void);
}

extern "C" void get_rand_32(uint32_t *r)
{
  *r = esp_random();
}

extern "C" ssize_t read(int fd, void *buf, size_t count)
{
  int i;
  ssize_t ret;
  uint32_t available;

  if (fd != STDIN_FILENO) {
    ret = -1;
    errno = 255;
    goto EXIT;
  }

  available = Serial.available();

  if (available == 0) {
    ret = -1;
    errno = EAGAIN;
    goto EXIT;
  }

  if (buf == NULL) {
    ret = available;
    goto EXIT;
  }

  if (available < count) {
    count = available;
  }

  for (ret = 0 ; ret < count ; ret++) {
    i = Serial.read();
    if (i == -1) {
      break;
    }
    ((uint8_t *)buf)[ret] = i;
  }

  if (ret == 0) {
    ret = -1;
    errno = 255;
  }

EXIT:

  return ret;
}

extern "C" ssize_t write(int fd, void const *buf, size_t count)
{
  size_t st;
  ssize_t ret;
  uint32_t available;

  if ((fd != STDOUT_FILENO) && (fd != STDERR_FILENO)) {
    ret = -1;
    errno = 255;
    goto EXIT;
  }

  available = Serial.availableForWrite();

  if (available == 0) {
    ret = -1;
    errno = EAGAIN;
    goto EXIT;
  }

  if (buf == NULL) {
    ret = available;
    goto EXIT;
  }

  if (available < count) {
    count = available;
  }

  for (ret = 0 ; ret < count ; ret++) {
    st = Serial.write(((uint8_t const *)buf)[ret]);
    if (st != 1) {
      break;
    }
  }

  if (ret == 0) {
    ret = -1;
    errno = 255;
  }

EXIT:

  return ret;
}

extern "C" int close(int fd)
{
  (void)fd;
  return 0;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Serial.printf("WiFi connected (%s)\n", WiFi.localIP().toString().c_str());
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned u;
  static bool isFin = false;
  if (!isFin) {
    Serial.printf("start\n\n");
    u = espress_stream_time();
    es_ccat_main();
    u = espress_stream_time() - u;
    printf("\nfinish (elapsed time = %u us)\n", u);
    isFin = true;
  }
}
