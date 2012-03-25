#!/usr/bin/perl
########################################################################################
# This file is part of the qx11grab project
#
# Copyright (C) Juergen Heinemann (Undefined) http://qx11grab.hjcms.de, (C) 2007-2012
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Library General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Library General Public License for more details.
#
# You should have received a copy of the GNU Library General Public License
# along with this library; see the file COPYING.LIB.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
# Boston, MA 02110-1301, USA.
########################################################################################

use 5.008;
use strict;
use warnings;
use Path::Class qw(file);
use Env qw(HJCMS_DOWNLOAD_DIR);

# current repository directory
my $repodir = file ( __FILE__ )->absolute()->parent();

# my download repository
my $destdir = $ENV{'HJCMS_DOWNLOAD_DIR'} . "/qx11grab";

# which branch
my $branch = 'master';

# get current QX11Grab version from CMakeLists.txt
sub fetch_version()
{
  my @vers;
  my $candidate = "";
  open ( FP, "CMakeLists.txt" ) or die ( "can't open CMakeLists.txt" );
  while ( <FP> )
  {
    if ( /QX11GRAB_VERSION_(MAJOR|MINOR|RELEASE)[\s]+([\d]+)/ )
    {
      push ( @vers, $2 );
    }
    if ( /QX11GRAB_VERSION_DEVELOPMENT[\s]+\"([\w]+)\"/ )
    {
      $candidate = ".$1";
    }
  }
  close ( FP );
  my $str_version = join ( ".", @vers );
  return $str_version . $candidate;
}

# current version
my $pkg_version = fetch_version();

# complite package name
my $pkg_name = "qx11grab-$pkg_version";

# create a clone in /tmp and clean up
print "- Clone to /tmp/$pkg_name\n";
system ( "rm", "-rf", "/tmp/$pkg_name" );
system ( "git", "clone", "-b", "$branch", "$repodir", "/tmp/$pkg_name" );
print "- Clearing /tmp/$pkg_name\n";
system ( "chgrp", "-R", "www", "/tmp/$pkg_name" );
system ( "rm", "-rf", "/tmp/$pkg_name/.git" );
# remove old version from download repository
system ( "rm", "-rf", "$destdir/$pkg_version" );
# create packages and move it to my download repository
system ( "mkdir", "-p", "$destdir/$pkg_version" );
chdir "/tmp";
print "- Generate $pkg_name.tar.bz2\n";
system ( "tar", "--exclude-vcs", "-cjf", "$destdir/$pkg_version/$pkg_name.tar.bz2", "$pkg_name" );
print "- Generate $pkg_name.tar.gz\n";
system ( "tar", "--exclude-vcs", "-czf", "$destdir/$pkg_version/$pkg_name.tar.gz", "$pkg_name" );
print "- Generate $pkg_name.tar.lzma\n";
system ( "tar", "--exclude-vcs", "-cJf", "$destdir/$pkg_version/$pkg_name.tar.lzma", "$pkg_name" );
print "- Generate $pkg_name.tar.xz\n";
system ( "tar", "--exclude-vcs", "-cJf", "$destdir/$pkg_version/$pkg_name.tar.xz", "$pkg_name" );
print "- Copy ChangeLog\n";
system ( "cp", "--update", "$repodir/ChangeLog", "$destdir/ChangeLog" );
chdir "$repodir";
print "- Ready\n";
