﻿////////////////////////////////////////////////////////////////////////////////////////////////////////
// Part of Injectable Generic Camera System
// Copyright(c) 2019, Frans Bouma
// All rights reserved.
// https://github.com/FransBouma/InjectableGenericCameraSystem
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met :
//
//  * Redistributions of source code must retain the above copyright notice, this
//	  list of conditions and the following disclaimer.
//
//  * Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and / or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED.IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
////////////////////////////////////////////////////////////////////////////////////////////////////////

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace IGCSClient.Classes
{
	public class KeyCombination
	{
		#region Members
		private string _textualRepresentation;
		private bool _altPressed;
		private bool _ctrlPressed;
		private bool _shiftPressed;
		private int _keyCode;
		private static List<string> KeyCodeToStringLookup = new List<string>()
															{
																"", "", "", "Cancel", "", "", "", "", "Backspace", "Tab", "", "", "Clear", "Enter", "", "",
																"Shift", "Control", "Alt", "Pause", "Caps Lock", "", "", "", "", "", "", "Escape", "", "", "", "",
																"Space", "Page Up", "Page Down", "End", "Home", "Left Arrow", "Up Arrow", "Right Arrow", "Down Arrow",
																"Select", "", "", "Print Screen", "Insert", "Delete", "Help",
																"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "", "", "", "", "", "",
																"", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O",
																"P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "Left Windows", "Right Windows", "", "", "Sleep",
																"Numpad 0", "Numpad 1", "Numpad 2", "Numpad 3", "Numpad 4", "Numpad 5", "Numpad 6", "Numpad 7", "Numpad 8", "Numpad 9",
																"Numpad *", "Numpad +", "", "Numpad -", "Numpad Decimal", "Numpad /",
																"F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12", "F13", "F14", "F15", "F16",
																"F17", "F18", "F19", "F20", "F21", "F22", "F23", "F24", "", "", "", "", "", "", "", "",
																"Num Lock", "Scroll Lock",
															};
		#endregion

		
		public KeyCombination()
		{
			_textualRepresentation = "Press a key";
			_altPressed = false;
			_ctrlPressed = false;
			_shiftPressed = false;
			_keyCode = 0;
		}


		public void CopyValues(KeyCombination toCopyFrom)
		{
			SetNewValues(toCopyFrom._altPressed, toCopyFrom._ctrlPressed, toCopyFrom._shiftPressed, toCopyFrom._keyCode);
		}


		public void Clear()
		{
			SetNewValues(false, false, false, 0);
		}


		public void SetNewValues(bool altPressed, bool ctrlPressed, bool shiftPressed, int keyCode)
		{
			_altPressed = altPressed;
			_shiftPressed = shiftPressed;
			_ctrlPressed = ctrlPressed;
			_keyCode = keyCode;

			CreateTextualRepresentation();
		}


		private void CreateTextualRepresentation()
		{
			var builder = new StringBuilder(512);
			if(_altPressed)
			{
				builder.Append("Alt+");
			}

			if(_ctrlPressed)
			{
				builder.Append("Ctrl+");
			}

			if(_shiftPressed)
			{
				builder.Append("Shift+");
			}

			if(_keyCode > 0)
			{
				builder.Append(GetTextualRepresentationOfKeyCode());
			}

			if(builder.Length <= 0)
			{
				builder.Append("Press a key");
			}
			_textualRepresentation = builder.ToString();
		}


		private string GetTextualRepresentationOfKeyCode()
		{
			if (_keyCode > 255 || _keyCode < 0)
			{
				return string.Empty;
			}
			return KeyCodeToStringLookup[_keyCode];
		}


		public override string ToString()
		{
			return _textualRepresentation;
		}
	}
}
