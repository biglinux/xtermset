# Maintainer    : Vilmar Catafesta <vcatafesta@gmail.com>
# description   : Change some settings of an xterm
# depends   	:

name=xtermset
version=0.5.2
release=1
pkgname=xtermset
pkgver=0.5.2
pkgrel=1
arch=('x86_64')
pkgdesc='Change some settings of an xterm'
url='https://github.com/biglinux/xtermset'
license=('GPL')
depends=('gcc')
source=(git+https://github.com/biglinux/xtermset)

prepare() {
	pkgdir=${PKG}
	srcdir=${SRC}
}

build() {
    cd src/xtermset/xtermset-0.5.2
    aclocal
    autoconf
	automake --add-missing --copy
    ./configure --prefix=/usr
    make
}

package() {
    cd xtermset/xtermset-0.5.2
    make DESTDIR=${pkgdir} install
}


