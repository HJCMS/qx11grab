# Maintainer: Juergen Heinemann (Undefined) http://qx11grab.hjcms.de
pkgname=qx11grab
pkgver=@QX11GRAB_VERSION_MAJOR@.@QX11GRAB_VERSION_MINOR@.@QX11GRAB_VERSION_RELEASE@
pkgrel=1
pkgdesc="a high flexible screencast Application for X11 Desktop"
url="http://qx11grab.hjcms.de"
license=('GPL3')
arch=('i686' 'x86_64')

depends=('qt' 'ffmpeg')
makedepends=('cmake' 'pkgconfig' 'qt' 'ffmpeg' 'dbus-1' 'x11' 'pulse' 'alsa' 'freetype2' 'fontconfig' 'desktop-file-utils')

source=(ftp://ftp.hjcms.de/qx11grab/${pkgver}/qx11grab-${pkgver}.tar.xz)
## wget ftp://ftp.hjcms.de/qx11grab/${pkgver}/qx11grab-${pkgver}.tar.xz.md5sum
md5sums=('')

build() {
  mkdir -p ${srcdir}/${pkgname}-${pkgver}/build
  cd ${srcdir}/${pkgname}-${pkgver}/build
  cmake -Wno-dev -DCMAKE_BUILD_TYPE:STRING=MinSizeRel -DCMAKE_INSTALL_PREFIX:PATH=/usr -DSYSCONF_INSTALL_DIR:PATH=/etc ../
  make ${smp_flags}
}

package() {
  cd ${srcdir}/${pkgname}-${pkgver}/build
  make DESTDIR=${pkgdir} install/fast
}
