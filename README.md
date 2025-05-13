## INTRO

Global Proxy App for Android System

ProxyDroid is distributed under GPLv3 with many other open source software, here is a list of them:

* cntlm - authentication proxy: http://cntlm.sourceforge.net/
* redsocks - transparent socks redirector: http://darkk.net.ru/redsocks/
* netfilter/iptables - NAT module: http://www.netfilter.org/
* transproxy - transparent proxy for HTTP: http://transproxy.sourceforge.net/
* stunnel - multiplatform SSL tunneling proxy: http://www.stunnel.org/

## Modifications

* Update gradle
* Update minimum sdk to 21
* Remove crashlytics
* Fixed libevent compilation with arc4random_addrandom
* Removed google services
* Remove deprecated wm.getConfiguredNetworks();

## Compilation

Tested with

* javac 22.0.2
* Gradle 8.13

```console
gradle build
# File written to ./app/build/outputs/apk/release/app-release-unsigned.apk
```

