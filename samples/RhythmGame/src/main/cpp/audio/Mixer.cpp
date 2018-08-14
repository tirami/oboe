/*
 * Copyright 2018 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <math.h>
#include "Mixer.h"

void Mixer::renderAudio(int16_t *audioData, int32_t numFrames) {

    // Zero out the incoming container array
    for (int j = 0; j < numFrames * kChannelCount; ++j) {
        audioData[j] = 0;
    }

    float interferenceOffset;
    int64_t ampSum;

    for (int i = 0; i < mNextFreeTrackIndex; ++i) {
        mTracks[i]->renderAudio(mixingBuffer, numFrames);

        for (int j = 0; j < numFrames * kChannelCount; ++j) {
            ampSum = audioData[j] + mixingBuffer[j];

            if ((ampSum > 32600) || (ampSum < -32600)){
                interferenceOffset = 32600 / fabs(ampSum);
                audioData[j] = int16_t(audioData[j] * interferenceOffset);
                audioData[j] += int16_t(mixingBuffer[j] * interferenceOffset);
            } else {
                audioData[j] += mixingBuffer[j];
            }
        }
    }
}

void Mixer::addTrack(RenderableAudio *renderer){
    mTracks[mNextFreeTrackIndex++] = renderer;
};