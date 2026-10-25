#ifdef __cplusplus
extern "C" {
#endif

int udp_begin(uint16_t port);
int udp_begin_packet(char const *address, uint16_t port);
size_t udp_write(uint8_t const *buffer, size_t size);
int udp_end_packet(void);
int udp_parse_packet(void);
size_t udp_read(uint8_t *buffer, size_t size);
char *udp_remote_ip(char *address, size_t size);
uint16_t udp_remote_port(void);

#ifdef __cplusplus
}
#endif
