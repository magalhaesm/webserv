# TODO

## ASCII Art
* elite
* DOS Rebel
* ANSI Shadow
* Graffiti
* Larry 3D 2
* Modular
* Ogre
*** Red Phoenix
 __      __        ___.      _________
/  \    /  \  ____ \_ |__   /   _____/  ____ _______ ___  __
\   \/\/   /_/ __ \ | __ \  \_____  \ _/ __ \\_  __ \\  \/ /
 \        / \  ___/ | \_\ \ /        \\  ___/ |  | \/ \   /
  \__/\  /   \___  >|___  //_______  / \___  >|__|     \_/
       \/        \/     \/         \/      \/

*** Soft
,--.   ,--.       ,--.    ,---.
|  |   |  | ,---. |  |-. '   .-'  ,---. ,--.--.,--.  ,--.
|  |.'.|  || .-. :| .-. '`.  `-. | .-. :|  .--' \  `'  /
|   ,'.   |\   --.| `-' |.-'    |\   --.|  |     \    /
'--'   '--' `----' `---' `-----'  `----'`--'      `--'

*** Standard
 __        __   _    ____
 \ \      / /__| |__/ ___|  ___ _ ____   __
  \ \ /\ / / _ \ '_ \___ \ / _ \ '__\ \ / /
   \ V  V /  __/ |_) |__) |  __/ |   \ V /
    \_/\_/ \___|_.__/____/ \___|_|    \_/


## Vault
- pedir nome e iniciar sessão
- encerrar sessão, esquecer

- [ ] checar e há cookie
- [ ] se já está logado, recebe a página principal (vault.html)
- [ ] se não está logado, mas tem cadastro recebe a página de login (login.html)
- [ ] se não está, recebe a página de cadastro (signup.html)

- [x] criar função setBody(body, cgi=true)
- [ ] renomear limit_except para methods e adicionar valor deny para bloquear acesso a um diretório



### Requisições Teste
curl -v -F "file=@Makefile.bkp" 127.0.0.1:8081/upload.py
curl -v -H "Transfer-Encoding: chunked" 127.0.0.1:8081/upload.py -F "file=@Makefile.bkp"
