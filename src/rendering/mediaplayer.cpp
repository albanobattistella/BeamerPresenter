// SPDX-FileCopyrightText: 2022 Valentin Bruch <software@vbruch.eu>
// SPDX-License-Identifier: GPL-3.0-or-later OR AGPL-3.0-or-later

#include <algorithm>
#include <QTimerEvent>
#include "src/rendering/mediaplayer.h"
#include "src/log.h"

void MediaPlayer::timerEvent(QTimerEvent *event)
{
    killTimer(event->timerId());
    timer_id = -1;
    checkPosition();
}

void MediaPlayer::checkPosition()
{
#if (QT_VERSION_MAJOR >= 6)
    debug_msg(DebugMedia, "check position" << seekpos << position() << mediaStatus() << playbackState());
#else
    debug_msg(DebugMedia, "check position" << seekpos << position() << mediaStatus() << state());
#endif
    /* It can happen that a video has duration()==0. In such a case I
     * experienced that setPosition() does not work. It only restarts
     * the video from the beginning. Therefore this disables seeking
     * if duration()==0. */
    if (seekpos >= 0 && isSeekable() && duration() > 0)
    {
        setPosition(std::min(seekpos, duration()));
        //setPosition(seekpos <= duration() || duration() == 0 ? seekpos : duration());
#if (QT_VERSION_MAJOR >= 6)
        debug_msg(DebugMedia, "done:" << position() << duration() << mediaStatus() << playbackState());
#else
        debug_msg(DebugMedia, "done:" << position() << duration() << mediaStatus() << state());
#endif
        seekpos = -1;
    }
}

void MediaPlayer::setPositionSoft(int position) noexcept
{
    seekpos = qint64(position);
    if (timer_id != -1)
        killTimer(timer_id);
    timer_id = startTimer(50);
}
