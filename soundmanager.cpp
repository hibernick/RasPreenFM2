//
// miniorgan.cpp
//
// Circle - A C++ bare metal environment for Raspberry Pi
// Copyright (C) 2017  R. Stange <rsta2@o2online.de>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
#include "soundmanager.h"
#include <circle/devicenameservice.h>
#include <circle/logger.h>
#include <assert.h>
#include "global_obj.h"
// #include "screenmanager.h"

#define VOLUME_PERCENT 100
#define MIDI_NOTE_OFF 0b1000
#define MIDI_NOTE_ON 0b1001

#define KEY_NONE 255

#define SINE_POINTS 360

static const char FromSoundManager[] = "SoundManager";

unsigned int preenTimer= 0;


CSoundManager *CSoundManager::s_pThis = 0;

CSoundManager::CSoundManager(CInterruptSystem *pInterrupt, CSerialMIDI *pm_SerialMIDI, Synth *psynth) : //	SOUND_CLASS (pInterrupt, SAMPLE_RATE),
													   SOUND_CLASS(pInterrupt, SAMPLE_RATE, CHUNKSIZE),
													   pm_SerialMIDI(pm_SerialMIDI),
													   m_nSampleCount(0),
													   m_nFrequency(0),
													   m_nPrevFrequency(0),
													   m_ucKeyNumber(KEY_NONE),
													   m_psynth(psynth)
{
	s_pThis = this;

}

CSoundManager::~CSoundManager(void)
{
	s_pThis = 0;
}

boolean CSoundManager::Initialize(void)
{

	return TRUE;
}

void CSoundManager::Process(void)
{
}


unsigned CSoundManager::GetChunk(u32 *pBuffer, unsigned nChunkSize)
{
	unsigned nResult = nChunkSize;

 	out_led3.Set(1);
	m_psynth->buildNewSampleBlock();
//	m_psynth->buildNewSampleBlock();
	// m_psynth->buildNewSampleBlock();
	// m_psynth->buildNewSampleBlock();
	out_led3.Set(0);
	for (; nChunkSize > 0; nChunkSize -= 2) // fill the whole buffer
	{
//		out_led3.Invert();
        preenTimer += 1;

        m_left = m_psynth->leftSampleAtReadCursor();
        m_right = m_psynth->rightSampleAtReadCursor();
        m_psynth->incReadCursor();

		*pBuffer++ = (u32)m_left << 3;
		*pBuffer++ = (u32)m_right << 3;
	}
	// out_led2.Set(0);

	return nResult;
}



