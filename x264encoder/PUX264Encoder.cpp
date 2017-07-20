#include "Buffer.h"
#include "PUX264Encoder.h"
#include "Macro.h"

void x264Encode::initX264Encode(int width, int height, int fps, int bite) {
	_x264_encoder = NULL;
	_x264_param = new x264_param_t;

	/**
	 x264_param_default(_x264_param);
	 x264_param_default_preset(_x264_param, "superfast", "zerolatency");
	 x264_param_apply_fastfirstpass(_x264_param);
	 x264_param_apply_profile(_x264_param, "baseline");

	 _x264_param->b_opencl = 1;
	 _x264_param->b_vfr_input = 0;

	 _x264_param->i_fps_num = _x264_param->i_timebase_num = fps
	 * 1000;
	 _x264_param->i_fps_den = _x264_param->i_timebase_den = 1000;
	 _x264_param->i_frame_total = 0;

	 _x264_param->i_keyint_min = fps;
	 _x264_param->i_keyint_max = fps;

	 _x264_param->i_log_level = X264_LOG_INFO;
	 //	_x264_param->pf_log = x264_log_vfw;
	 _x264_param->p_log_private = NULL;

	 _x264_param->i_width = width;
	 _x264_param->i_height = height;

	 xfps = fps;
	 xheight = height;
	 xwidth = width;**/

	//origin**
	x264_param_default_preset(_x264_param, "fast", "zerolatency");

	if (bite == 0) {
		bite = 1;
	}
	if (bite > 0 && bite <= 64) {
		bitratelevel = BIT_LOW_LEVEL;
	} else if (bite > 64 && bite <= 128) {
		bitratelevel = BIT_MEDIUM_LEVEL;
	} else if (bite > 128 && bite <= 256) {
		bitratelevel = BIT_STANDARD_LEVEL;
	} else if (bite > 256 && bite <= 384) {
		bitratelevel = BIT_HIGH_LEVEL;
	} else if (bite > 384 && bite <= 512) {
		bitratelevel = BIT_HIGH_LEVEL;
	} else {
		bitratelevel = BIT_STANDARD_LEVEL;
	}
	if (bitratelevel == BIT_LOW_LEVEL) {
		_x264_param->rc.f_rf_constant = 32;
	} else if (bitratelevel == BIT_MEDIUM_LEVEL) {
		_x264_param->rc.f_rf_constant = 29;
	} else if (bitratelevel == BIT_STANDARD_LEVEL) {
		_x264_param->rc.f_rf_constant = 26;
	} else if (bitratelevel == BIT_HIGH_LEVEL) {
		_x264_param->rc.f_rf_constant = 24;
	} else {
		_x264_param->rc.f_rf_constant = 24;
	}

	_x264_param->b_repeat_headers = 1; // 重复SPS/PPS 放到关键帧前面
	_x264_param->rc.i_rc_method = X264_RC_CQP; //CQP(∫„∂®÷ ¡ø)£¨CRF(∫„∂®¬Î¬ )£¨ABR(∆Ωæ˘¬Î¬ )

	_x264_param->i_width = width;
	_x264_param->i_height = height;
	_x264_param->i_frame_total = 0; // 编码总帧数.不知道用0.
	_x264_param->i_keyint_max = fps * 2;
	_x264_param->i_keyint_min = fps;

	_x264_param->i_fps_den = 1; // 帧率分母
	_x264_param->i_fps_num = fps; // 帧率分子
	_x264_param->i_timebase_den = _x264_param->i_fps_num;
	_x264_param->i_timebase_num = _x264_param->i_fps_den;

	_x264_param->i_cqm_preset = X264_CQM_FLAT;

	xfps = fps;
	xheight = height;
	xwidth = width;

	_x264_param->analyse.i_me_method = X264_ME_HEX;
	_x264_param->analyse.i_subpel_refine = 2;
	_x264_param->i_frame_reference = 1;
	_x264_param->analyse.b_mixed_references = 0;
	_x264_param->analyse.i_trellis = 0;
	_x264_param->b_sliced_threads = 0;
	_x264_param->i_threads = 1;
	//i_threads = N并行编码的时候如果b_sliced_threads=1那么是并行slice编码，
	//如果b_sliced_threads=0，那么是并行frame编码。并行slice无延时，并行frame有延时

	_x264_param->analyse.b_transform_8x8 = 0;
	_x264_param->b_cabac = 1;
	_x264_param->b_deblocking_filter = 1;
	_x264_param->psz_cqm_file = NULL;
	_x264_param->analyse.i_weighted_pred = X264_WEIGHTP_NONE;
	_x264_param->rc.i_lookahead = 0;
	_x264_param->i_bframe = 0;

	x264_param_apply_profile(_x264_param, "baseline");

	/**
	 _x264_param->i_width = width;
	 _x264_param->i_height = height;
	 _x264_param->b_repeat_headers = 1;  // 重复SPS/PPS 放到关键帧前面
	 _x264_param->b_cabac = 1;
	 _x264_param->i_threads = 1;
	 _x264_param->i_fps_num = fps;
	 _x264_param->i_fps_den = 1;
	 _x264_param->i_keyint_max = fps * 2;

	 // rc
	 _x264_param->rc.b_mb_tree=0;//这个不为0,将导致编码延时帧->->->在实时编码时,必须为0
	 _x264_param->rc.f_rf_constant = 25;
	 _x264_param->rc.f_rf_constant_max = 45;
	 _x264_param->rc.i_rc_method = X264_RC_ABR;//参数i_rc_method表示码率控制，CQP(恒定质量)，CRF(恒定码率)，ABR(平均码率)
	 //param->rc->f_rate_tolerance=0->1;
	 _x264_param->rc.i_vbv_max_bitrate=(int)((bite*1.2)/1000) ; // 平均码率模式下，最大瞬时码率，默认0(与-B设置相同)

	 _x264_param->rc.i_bitrate = (int)bite/1000;


	 x264_param_apply_profile(_x264_param, "baseline");
	 _x264_param->i_level_idc=30;

	 _x264_param->i_log_level = X264_LOG_NONE;**/

#ifdef __ANDROID__
	_x264_encoder = x264_encoder_open(_x264_param);

#else
	_x264_encoder = x264_encoder_open_133(_x264_param);
#endif
	if (_x264_encoder == NULL) {
//            NSLog(@"encoder error");
	}
	int iMaxFrames = x264_encoder_maximum_delayed_frames(_x264_encoder);

//        NSLog(@"缓存最大帧数：%d",iMaxFrames);
	_in_pic = new x264_picture_t;
	_out_pic = new x264_picture_t;
	x264_picture_init(_out_pic);
	x264_picture_alloc(_in_pic, X264_CSP_I420, _x264_param->i_width,
			_x264_param->i_height);
	_in_pic->img.i_csp = X264_CSP_I420;
	_in_pic->img.i_plane = 3;

//    x264_picture_alloc(_in_pic, X264_CSP_BGRA, _x264_param->i_width, _x264_param->i_height);
//    _in_pic->img.i_csp = X264_CSP_BGRA;
//    _in_pic->img.i_plane = 3;
}

void x264Encode::startEncoder(uint8_t * dataptr, char **&bufdata, int *&bufdatalen ,int &buflen,
		int &isKeyFrame) {

	int width = xheight;
	int height = xwidth;

	picture_buf = dataptr;
#ifdef __ANDROID__
#else

	UInt8 *pUV = picture_buf + width * height;
	UInt8 *tmp = (UInt8 *) malloc(width * height / 2);
	memcpy(tmp, pUV, width * height / 2);

	int fsize = width * height / 4;
	UInt8 *pU = pUV;
	UInt8 *pV = pUV + fsize;
	for (int i = 0; i < fsize; i++) {
		*pU = tmp[i << 1];
		*pV = tmp[(i << 1) + 1];
		pV++;
		pU++;
	}
	free(tmp);
#endif
	//_in_pic.img.i_plane = 3;

	memcpy(_in_pic->img.plane[0], picture_buf, width * height);
	memcpy(_in_pic->img.plane[1], picture_buf + width * height,
			width * height / 4);
	memcpy(_in_pic->img.plane[2], picture_buf + width * height * 5 / 4,
			width * height / 4);

	_in_pic->i_type = X264_TYPE_AUTO;

	_in_pic->i_qpplus1 = 0;
	_in_pic->param = _x264_param;
	_in_pic->i_pts = 0;
	x264_nal_t *nal;
	int i_nal = 0;
	int Result;
	if (_x264_encoder != NULL) {

		Result = x264_encoder_encode(_x264_encoder, &nal, &i_nal, _in_pic,
				_out_pic);
		isKeyFrame = _out_pic->b_keyframe;
		_in_pic->i_pts++;
	}

	//NSLog(@"Encode Time：%d",clock()-t);
	//printf("\r\nEncode Time: %d\r\n", clock()-t);
	if (Result < 0) {
//            NSLog(@"encoder faild");
		//LOGI("/**********************encoder faild************************");
	} else if (Result == 0) {
		//NSLog(@"编码成功,但被缓存了");
		LOGI("/**********************编码成功,但被缓存了************************");
	} else {
		//NSLog(@"得到编码数据");
		/*/ origin
		 int bufsize = 0;
		 for (int i = 0; i < i_nal; i++) {
		 bufsize += nal[i].i_payload;
		 }

		 char * tempdata = new char[bufsize];
		 memset(tempdata, 0, bufsize);
		 bufdata = tempdata;
		 for (int i = 0; i < i_nal; i++) {
		 if (nal[i].p_payload != NULL) {
		 memcpy(tempdata, nal[i].p_payload, nal[i].i_payload);
		 tempdata += nal[i].i_payload;
		 }
		 }
		 LOGD("tempdata.size=%d",strlen(tempdata));
		 buflen = bufsize;		/*/
		//LOGI("/**********************编码成功,长度为：%d************************",bufsize);
		//
		if (i_nal) {
			char **tempdata = new char*[i_nal];
			int *datalen = new int[i_nal];
			for (int i = 0; i < i_nal; i++) {
				char *temp = new char[nal[i].i_payload];
				memset(temp, 0, nal[i].i_payload);
				if (nal[i].p_payload != NULL) {
					memcpy(temp, nal[i].p_payload, nal[i].i_payload);
					//temp += nal[i].i_payload;
				}
				LOGD("new temp :%d, i_nal :%d, temp size :%d", i,i_nal,nal[i].i_payload);
				tempdata[i] = temp;
				datalen[i] = nal[i].i_payload;
			}
			bufdata = tempdata;
			bufdatalen = datalen;
		}

		buflen = i_nal;		//
	}
}

void x264Encode::Flush() {
	x264_picture_t pic_out;
	x264_nal_t * nal;
	int i_nal;
	if (x264_encoder_encode(_x264_encoder, &nal, &i_nal, NULL, &pic_out) < 0) {
	}
}

void x264Encode::releaseEncoder() {
	this->Flush();

	x264_picture_clean(_in_pic);
//x264_picture_clean(_out_pic);

//* 关闭编码器句柄

	x264_encoder_close(_x264_encoder);
	_x264_encoder = NULL;

	if (_in_pic != NULL) {
		delete _in_pic;
		_in_pic = NULL;
	}

	if (_out_pic != NULL) {
		delete _out_pic;
		_out_pic = NULL;
	}

	if (_x264_param != NULL) {
		delete _x264_param;
		_x264_param = NULL;
	}

}

