# Certificados de confiança

`cacert.pem`: lista pública de raízes Mozilla distribuída por https://curl.se/ca/cacert.pem. O cabeçalho do arquivo registra data, origem e licença.

`x509_crt_bundle.bin`: convertido com `scripts/make_bundle.py`, formato de subjects/SPKI ordenados compatível com Arduino-ESP32 2.0.17. Incluído na flash por PlatformIO e usado por WiFiClientSecure.

Para atualizar, obtenha PEM pela fonte HTTPS confiável e execute:

```sh
python -m pip install cryptography
python scripts/make_bundle.py data/cert/cacert.pem data/cert/x509_crt_bundle.bin
pio run
```

Incluir uma CA privada implica confiar nela para as conexões feitas pelo firmware. Nunca substituir por certificados obtidos de um endpoint desconhecido. Não usar `setInsecure` para contornar falhas de horário/cadeia.
