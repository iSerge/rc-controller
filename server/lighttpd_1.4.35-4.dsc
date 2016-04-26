-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (quilt)
Source: lighttpd
Binary: lighttpd, lighttpd-doc, lighttpd-mod-mysql-vhost, lighttpd-mod-trigger-b4-dl, lighttpd-mod-cml, lighttpd-mod-magnet, lighttpd-mod-webdav
Architecture: any all
Version: 1.4.35-4
Maintainer: Debian lighttpd maintainers <pkg-lighttpd-maintainers@lists.alioth.debian.org>
Uploaders: Krzysztof Krzyżaniak (eloy) <eloy@debian.org>, Olaf van der Spek <olafvdspek@gmail.com>, Arno Töll <arno@debian.org>, Michael Gilbert <mgilbert@debian.org>
Homepage: http://lighttpd.net/
Standards-Version: 3.9.5
Vcs-Browser: http://anonscm.debian.org/gitweb/?p=pkg-lighttpd/lighttpd.git
Vcs-Git: git://anonscm.debian.org/pkg-lighttpd/lighttpd.git
Build-Depends: dpkg-dev (>= 1.16.1~), debhelper (>= 9.20130624~), dh-systemd (>= 1.3), dh-autoreconf, mime-support, libssl-dev, zlib1g-dev, libbz2-dev, libattr1-dev, libpcre3-dev, libmysqlclient-dev, libfam-dev, libldap2-dev, libfcgi-dev, libgdbm-dev, liblua5.1-0-dev, pkg-config, uuid-dev, libsqlite3-dev, libxml2-dev, libkrb5-dev, perl
Package-List:
 lighttpd deb httpd optional arch=any
 lighttpd-doc deb doc optional arch=all
 lighttpd-mod-cml deb httpd optional arch=any
 lighttpd-mod-magnet deb httpd optional arch=any
 lighttpd-mod-mysql-vhost deb httpd optional arch=any
 lighttpd-mod-trigger-b4-dl deb httpd optional arch=any
 lighttpd-mod-webdav deb httpd optional arch=any
Checksums-Sha1:
 90c22d55c9656494d772deb62e253aa35bb5221d 847321 lighttpd_1.4.35.orig.tar.gz
 5ed245544f8ba0889f09484a19383ad520f8bcd2 26112 lighttpd_1.4.35-4.debian.tar.xz
Checksums-Sha256:
 62c23de053fd82e1bf64f204cb6c6e44ba3c16c01ff1e09da680d982802ef1cc 847321 lighttpd_1.4.35.orig.tar.gz
 3ed7e007f2363220ca6c50333c04853f6207093a1355535567de6ded2e2d5cce 26112 lighttpd_1.4.35-4.debian.tar.xz
Files:
 69057685df672218d45809539b874917 847321 lighttpd_1.4.35.orig.tar.gz
 9fd446785e59581500a122975ac700c5 26112 lighttpd_1.4.35-4.debian.tar.xz

-----BEGIN PGP SIGNATURE-----
Version: GnuPG v1

iQQcBAEBCgAGBQJUVZ6SAAoJELjWss0C1vRzPAYgAI3SESYCrpSfIPAUx3tBqUeJ
FV+lej8YqoS7RFCFrIF0BB8/hYYfkY2BIp7dc89aSCKX/c17GGBor//TWIMvLA3s
FOhffZ+KVGgWp3q4uvr75xWIMOtF9r6PhiljqwC4Vn8SSyL9TclP6RZQSmOhsRu1
MIGrg+5xpsWtRkY+UDTz4/mqaAsJQrg2LX7Ajg/qqWZ2zIbwPTq070Hs7WkZaSVW
qTOWvcr6lGdD6S45sHIGnwS/tH9jca9mBGB2jN1ACVqwkK/e7I7fAmaPcOT4Mfbr
wU4NdwYzA6R+zVl23HwP32IGsagb8lmAkWX6IREGbvwpQJ5kGcK4FYU8vfK0iOvA
WW0kVZ2F9SSmzqklAw8Wy0/+iGR40aGRKHqVzRuQcTACWYu0/p/nNXbXM9e6qLLe
OjPheNnQ86lKOdPIX2qtvoM5oVRQgxEVkTfuzuYFbnttPeJc4VHvxlGshcW14ixK
oBqoCiVT+hXmoTmRap4mbFfUkEChebGVb0y9TXDGO9PF5s1vbRX0WctcKGTzFS6O
kY4p0wRJonVz5fEBJhEdmVe4aLQ8rJm1fy+Q8e64XhiivtL+XI6qTF6E+XKVvaUx
YJWnHTqfaW+C4RF1q//nO71fJt9A4zaw+KfoLFH3AYiS2c0lyDQyKan/EHksooDY
wd3+eTARs1FcJj/QFr8yXs4Cku1mbLnJlAVjUNfjN6x4zKkav9sa0di6r09CT6MD
Wl8ikts1PbcenL+ZjUX7uKR7jzPUzviM8OpRcf244X7oAtTwSuOev1L7zyvygHCT
weWbuo13PbwtbfXL53LSPrGPrVckngbrntQ1OIRyHJABDyaYGZ+hDiQpMEhDRhbz
5Of8qNFP3WtYauTquAtIRQBURAmT4ZUPuqL4Ima4NmvO2ceQ4oXpYa/Xk0E0Nyrt
uNgrpH++e8ZyOSAUF3EyuINPnZwX3VHZeznoyBf3X2DA9QNZnMOuU2r/8NAl2d7H
UGNJrFDq06x21QY1EpErJ8ZV9p1LLPOjnicTHe3L+w9u0lLjWNP3M3C1b1TwkF4d
V6Neqja3xa4epOHGcJPZOal5w8MfNx3YkCuJBLKvuOU4XPh6UHvjki69Ka3RfLUs
2R/Yxhap/VmnLUerVMHBl1gLTvsDbHr44ZTsLbuYN8TMMW6QsliPoH0cqbwVijNG
NFX8yzNkjQZMXxCE0QGK9po/a+Qo3mwq3Rd9XZA0R+92YXOKXee9jOkooPv3Tdq6
GJhUT2RzmKVfjNL8zS/TxJPay0EUoOeOvUx3U0B5zmYYIFXJU/KwCiSr5DvoSc+7
kNRNAWWM6leQ8YcdlAUIskiItykmcvFoAkGbRMdGhItlOjG2OVH9NRquhD8vvUQ=
=5h9j
-----END PGP SIGNATURE-----
