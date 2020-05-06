//###################################################################
//		���j���[�V�[��: �I�v�V�������j���[
//###################################################################

///[summary]�I�v�V�������ڂ̕`�揈��
function DrawOptionMenu() {
	for(local i = 0; i < OptionMenu.len(); i++) {
		switch(i) {
			case OptionMenu.MapMessageSpeed:
				Game.DXLib.DrawString(240, 20 + 25 * i, 0x2E19E6A3/*�Z�[�u�f�[�^���X�g�p*/, WHITE,
					"��b���b�Z�[�W�\�����x"
				);
				Game.DXLib.DrawString(450, 20 + 25 * i, 0x2E19E6A3/*�Z�[�u�f�[�^���X�g�p*/, WHITE,
					format("[ %d ]", UI2084302691/*��b���b�Z�[�W�\�����x*/)
				);
				break;
			case OptionMenu.BattleMessageSpeed:
				Game.DXLib.DrawString(240, 20 + 25 * i, 0x2E19E6A3/*�Z�[�u�f�[�^���X�g�p*/, WHITE,
					"�퓬���b�Z�[�W�\�����x"
				);
				Game.DXLib.DrawString(450, 20 + 25 * i, 0x2E19E6A3/*�Z�[�u�f�[�^���X�g�p*/, WHITE,
					format("[ %d ]", UI315411244/*�퓬���b�Z�[�W�\�����x*/)
				);
				break;
			case OptionMenu.SoundVolume:
				Game.DXLib.DrawString(240, 20 + 25 * i, 0x2E19E6A3/*�Z�[�u�f�[�^���X�g�p*/, WHITE,
					"�T�E���h����"
				);
				Game.DXLib.DrawString(450, 20 + 25 * i, 0x2E19E6A3/*�Z�[�u�f�[�^���X�g�p*/, WHITE,
					format("[%3d]", Game.Material.Sounds.StandardVolume)
				);
				break;
		}
	}
	if(Depth > 0) {
		DrawCursorPanel(200 + 15, 12 + 4 + 25 * Cursor[1], Game.WindowSize.Width - 200 - 15 * 2, 22, true);
	}
}


///[summary]�I�v�V�������ڏ�̓��͏���
function InputOptionMenu() {
	if(CheckUpKeyInput() == true) {
		Cursor[Depth] = Game.DownAround(Cursor[Depth], 0, OptionMenu.len());
	} else if(CheckDownKeyInput() == true) {
		Cursor[Depth] = Game.UpAround(Cursor[Depth], 0, OptionMenu.len());
	} else if(CheckDecideKeyInput() == 1) {
		Game.DB.FDBMaterial.Play(0x61301713/*���艹*/, 255, 0, 0);
		Depth = 0;
	} else if(CheckCancelKeyInput() == 1) {
		Game.DB.FDBMaterial.Play(0x79C24514/*�L�����Z����*/, 255, 0, 0);
		Depth = 0;
	} else if(CheckLeftKeyInput() == true) {
		//�ݒ�l�_�E��
		switch(Cursor[1]) {
			case OptionMenu.MapMessageSpeed:
				if(0 <= UI2084302691/*���b�Z�[�W�\�����x*/ - 1) {
					UI2084302691/*���b�Z�[�W�\�����x*/--;
				}
				break;
			case OptionMenu.BattleMessageSpeed:
				if(0 < UI315411244/*�퓬���b�Z�[�W�\�����x*/ - 1) {
					UI315411244/*�퓬���b�Z�[�W�\�����x*/--;
				}
				break;
			case OptionMenu.SoundVolume:
				if(0 < Game.Material.Sounds.StandardVolume) {
					Game.Material.Sounds.StandardVolume--;
				}
				if(Game.SceneMap.BGM.Handle != -1) {
					Game.Material.Sounds.Pause(Game.SceneMap.BGM.Handle, 0);
					Game.Material.Sounds.Play(
						Game.SceneMap.BGM.Handle, PlayType.Loop,
						Game.Material.Sounds.StandardVolume, Game.SceneMap.BGM.Pitch, Game.SceneMap.BGM.Panpot, 0);
					Game.Material.Sounds.Resume(Game.SceneMap.BGM.Handle, true, 0);
				}
				if(Game.SceneMap.BGS.Handle != -1) {
					Game.Material.Sounds.Pause(Game.SceneMap.BGS.Handle, 0);
					Game.Material.Sounds.Play(
						Game.SceneMap.BGS.Handle, PlayType.Loop,
						Game.Material.Sounds.StandardVolume, Game.SceneMap.BGM.Pitch, Game.SceneMap.BGM.Panpot, 0);
					Game.Material.Sounds.Resume(Game.SceneMap.BGS.Handle, true, 0);
				}
				break;
		}
	} else if(CheckRightKeyInput() == true) {
		//�ݒ�l�A�b�v
		switch(Cursor[1]) {
			case OptionMenu.MapMessageSpeed:
				if(UI2084302691/*���b�Z�[�W�\�����x*/ + 1 < 10) {
					UI2084302691/*���b�Z�[�W�\�����x*/++;
				}
				break;
			case OptionMenu.BattleMessageSpeed:
				if(UI315411244/*�퓬���b�Z�[�W�\�����x*/ + 1 < 10) {
					UI315411244/*�퓬���b�Z�[�W�\�����x*/++;
				}
				break;
			case OptionMenu.SoundVolume:
				if(Game.Material.Sounds.StandardVolume + 1 <= 255) {
					Game.Material.Sounds.StandardVolume++;
				}
				if(Game.SceneMap.BGM.Handle != -1) {
					Game.Material.Sounds.Pause(Game.SceneMap.BGM.Handle, 0);
					Game.Material.Sounds.Play(
						Game.SceneMap.BGM.Handle, PlayType.Loop,
						Game.Material.Sounds.StandardVolume, Game.SceneMap.BGM.Pitch, Game.SceneMap.BGM.Panpot, 0);
					Game.Material.Sounds.Resume(Game.SceneMap.BGM.Handle, true, 0);
				}
				if(Game.SceneMap.BGS.Handle != -1) {
					Game.Material.Sounds.Pause(Game.SceneMap.BGS.Handle, 0);
					Game.Material.Sounds.Play(
						Game.SceneMap.BGS.Handle, PlayType.Loop,
						Game.Material.Sounds.StandardVolume, Game.SceneMap.BGM.Pitch, Game.SceneMap.BGM.Panpot, 0);
					Game.Material.Sounds.Resume(Game.SceneMap.BGS.Handle, true, 0);
				}
				break;
		}
	}
}
