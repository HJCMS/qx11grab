/*
* This file is part of the qx11grab project
*
* Copyright (C) Juergen Heinemann (Undefined) http://qx11grab.hjcms.de, (C) 2007-2012
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Library General Public
* License as published by the Free Software Foundation; either
* version 2 of the License, or (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Library General Public License for more details.
*
* You should have received a copy of the GNU Library General Public License
* along with this library; see the file COPYING.LIB.  If not, write to
* the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
* Boston, MA 02110-1301, USA.
*/

#include "videoinfodialog.h"

/* QtCore */
#include <QtCore/QDateTime>
#include <QtCore/QDebug>
#include <QtCore/QFileInfo>
#include <QtCore/QTime>

/* QtGui */
#include <QtGui/QDialogButtonBox>
#include <QtGui/QFormLayout>
#include <QtGui/QVBoxLayout>

/* FFmpeg */
extern "C"
{
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavutil/samplefmt.h>
#include <libavutil/pixdesc.h>
#include <libavutil/avstring.h>
#include <libavutil/opt.h>
#include <libavutil/dict.h>
#include <libavutil/parseutils.h>
}

namespace QX11Grab
{
  VideoInfoDialog::VideoInfoDialog ( QWidget * parent )
      : QDialog ( parent )
  {
    setObjectName ( QLatin1String ( "VideoInfoDialog" ) );
    /*: WindowTitle */
    setWindowTitle ( trUtf8 ( "Video File Information" ) );
    setContentsMargins ( 5, 5, 5, 15 );
    setMinimumSize ( 350, 200 );
    setSizeGripEnabled ( true );

    QVBoxLayout* layout = new QVBoxLayout ( this );

    QWidget* formWidget = new QWidget ( this );
    layout->addWidget ( formWidget );

    QFormLayout* formLayout = new QFormLayout ( formWidget );

    m_vCodecName = new QLabel ( formWidget );
    m_vCodecName->setText ( trUtf8 ( "Unknown" ) );
    formLayout->addRow ( trUtf8 ( "Video:" ), m_vCodecName );

    m_aCodecName = new QLabel ( formWidget );
    formLayout->addRow ( trUtf8 ( "Audio:" ), m_aCodecName );

    m_bitRate = new QLabel ( formWidget );
    formLayout->addRow ( trUtf8 ( "Bitrate:" ), m_bitRate );

    m_fileName = new QLabel ( formWidget );
    formLayout->addRow ( trUtf8 ( "File:" ), m_fileName );

    m_fileModified = new QLabel ( formWidget );
    formLayout->addRow ( trUtf8 ( "Modified:" ), m_fileModified );

    m_videoSize = new QLabel ( formWidget );
    formLayout->addRow ( trUtf8 ( "Size:" ), m_videoSize );

    // next Row...

    formWidget->setLayout ( formLayout );

    layout->addStretch ( 1 );

    QDialogButtonBox* m_buttonBox = new QDialogButtonBox ( QDialogButtonBox::Ok, Qt::Horizontal, this );
    m_buttonBox->setCenterButtons ( true );
    layout->addWidget ( m_buttonBox );

    setLayout ( layout );

    connect ( m_buttonBox, SIGNAL ( accepted () ), this, SLOT ( accept() ) );
  }

  void VideoInfoDialog::setBitrateLabel ( int r )
  {
    if ( r < 1 )
    {
      m_bitRate->clear();
      return;
    }
    int rate = qRound ( r / 1000 );
    m_bitRate->setText ( trUtf8 ( "%1 kb/s" ).arg ( QString::number ( rate ) ) );
  }

  void VideoInfoDialog::setFileInfos ( const QString &p )
  {
    QFileInfo inf ( p );
    m_fileName->setText ( inf.fileName() );
    QString lm = inf.lastModified().toString ( Qt::DefaultLocaleLongDate );
    m_fileModified->setText ( lm );
  }

  void VideoInfoDialog::setSizeInfo ( int w, int h )
  {
    QString s;
    s.append ( QString::number ( w ) );
    s.append ( "x" );
    s.append ( QString::number ( h ) );
    m_videoSize->setText ( s );
  }

  bool VideoInfoDialog::queryFullFileInfo ( const QByteArray &p )
  {
    av_register_all();
    AVFormatContext* af_ctx = avformat_alloc_context();
    const char* file = p.constData();

    if ( avformat_open_input ( &af_ctx, file, NULL, NULL ) < 0 )
    {
      avformat_free_context ( af_ctx );
      return false;
    }

    AVDictionary* dict = NULL;
    if ( avformat_find_stream_info ( af_ctx, &dict ) >= 0 )
    {
      AVInputFormat* frt = af_ctx->iformat;
      m_vCodecName->setText ( QString::fromUtf8 ( frt->long_name ) );
      setBitrateLabel ( af_ctx->bit_rate );
      AVCodecContext* c_ctx = af_ctx->streams[0]->codec;
      if ( c_ctx )
      {
        setSizeInfo ( c_ctx->width, c_ctx->height );
        // TODO
      }
    }
    avformat_close_input ( &af_ctx );
    return true;
  }

  int VideoInfoDialog::exec ( const QString &file )
  {
    if ( queryFullFileInfo ( file.toUtf8() ) )
    {
      setFileInfos ( file );
      return QDialog::exec();
    }
    return QDialog::Rejected;
  }

  VideoInfoDialog::~VideoInfoDialog()
  {
  }
}  /* eof namespace QX11Grab */
