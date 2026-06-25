/**
  ******************************************************************************
  * @file    hello_world_data_params.c
  * @author  AST Embedded Analytics Research Platform
  * @date    2026-06-25T21:01:17+0800
  * @brief   AI Tool Automatic Code Generator for Embedded NN computing
  ******************************************************************************
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  ******************************************************************************
  */

#include "hello_world_data_params.h"


/**  Activations Section  ****************************************************/
ai_handle g_hello_world_activations_table[1 + 2] = {
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
  AI_HANDLE_PTR(NULL),
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
};




/**  Weights Section  ********************************************************/
AI_ALIGNED(32)
const ai_u64 s_hello_world_weights_array_u64[239] = {
  0x7f817f817f818181U, 0x7f817f7f817f7f7fU, 0x7f817f817f7f7f81U, 0x7f7f81818181817fU,
  0x0U, 0xffffeb0900000000U, 0x6c9300000000U, 0xfffff8ae00000000U,
  0xffffea0e00000facU, 0x14288U, 0xffffc642ffffc614U, 0xffffef8b00000000U,
  0xffffe69600000000U, 0x4b400000336U, 0x14eb00000000U, 0xffffeaf600000000U,
  0xffffc1a4U, 0x0U, 0x0U, 0xfffffb75ffffb27aU,
  0xbfb40eaca14a2a38U, 0xed40b17cd6fb90cU, 0xcc1febd6861f9ec5U, 0x81f0c9abc3eade36U,
  0x3d083e584cbaebeeU, 0xdfee76f4635d52fbU, 0x39372a813a4470c7U, 0xd0162dda073c8eeeU,
  0x141113ecf7e50d13U, 0xdc05c8f30d0a0c01U, 0xde09fdf115ee0613U, 0xfc8108ec04e8f7e8U,
  0xc2435e5f9b3503d4U, 0x6ea459c66e816df2U, 0x678f61e548197cU, 0x9323a4932cee3b46U,
  0xe6132d272d03c6eaU, 0xbc0d1d2bd924cce9U, 0x13290902c8e72b43U, 0x8102e63341e5c1d2U,
  0xe6d3e048050a89ddU, 0x7f1b2e19b54ab45eU, 0x428fbab176a4d753U, 0x6cc1480b8388c00dU,
  0xbce9d110e9ee1ecbU, 0x20d9df201e48f00bU, 0x81eb2939fcf956efU, 0xa6b30ed23e062200U,
  0x60e003f5bf5aeeaU, 0x286d0a4c8acf5363U, 0xf1a59322306f9c41U, 0x93366d793e151b7fU,
  0x6024ff6450dde3e3U, 0xb800cf405a33dfe9U, 0x3820dfbdc80e7e7fU, 0xa7670051c5f1459fU,
  0xd227e86ab4c0c3d1U, 0x58cff74a09712ee6U, 0xc7240db35339657aU, 0x5ec21949a1a6aa81U,
  0xa757dd869cdad37fU, 0x53bed326a2a1a028U, 0xf6d342de0edbc797U, 0x54f77ebd013284c5U,
  0xda1e251705fcef06U, 0xedfdf6e2e201d71dU, 0xeb1f21ef120eddeeU, 0x3811ee6f013ffb6U,
  0xcafd28e809f2060bU, 0xb14e8d4ef2507daU, 0x17e9faf0d7f6f904U, 0x81fc26031b081ef4U,
  0xba64cd3f4d64021cU, 0xab8171c904134007U, 0x9a0e8463f28f3796U, 0xc94dc019203931b9U,
  0xe042fb2d0c1f16cfU, 0xabd3682030ef4b4cU, 0x7c4a4c481178a5bU, 0xc5e174e2e779ad3dU,
  0xe6e1b7fed84308cdU, 0xbecdb8b2354dcd14U, 0x1cdbd5e6431bb314U, 0x29810ce1ef2625e8U,
  0x5174b747e9463116U, 0xf13e769496017821U, 0x65cc0ca171777fe8U, 0xedef853a13d5566aU,
  0x62a83b27d0d3cec1U, 0x44c69ba23c644b6aU, 0xfe686c38835652e2U, 0xe68198a250c0ffadU,
  0x3ab2d3356fee26f2U, 0x8143b7ce6f31f56dU, 0x2d1a9731d3c4d54aU, 0x6886473e425d4351U,
  0xd0dcef73a7d688bcU, 0xc84d42fce6bdcb0dU, 0x7cd724ad3781639fU, 0x89947118f245f6a8U,
  0x6e5a445089ffe415U, 0x388f39f1314a31f9U, 0xcc81190138e98eadU, 0xf5824606962f6312U,
  0xcdd92cbc0f156caU, 0xad971910b2815917U, 0xdb4f1b1df6685deU, 0xd4d0b77fe1605440U,
  0xfed7927fa5312e32U, 0x139eb0d39a85514U, 0xe366f8e1b90fad96U, 0xdd3eb89171c4d7fU,
  0xc84619e3d1b78c8bU, 0xb47f2c8ec0759f85U, 0xbe062af7aaaf2153U, 0x4582bbc04dbc21U,
  0xcccf63a690615b1fU, 0xe3ed2deda8b7dc46U, 0x2b5bb5294c8194fbU, 0x6231afc352204d5dU,
  0x1345fdcd03ccca64U, 0x8be74cb60d5acc03U, 0x9bd279d612498129U, 0xcfabc8e7cb2e3daaU,
  0x2043b614aa7f8a0dU, 0xcdeaeccdbcd12097U, 0x2f5d93e0eea31d6aU, 0x8d17e035ee7cefb3U,
  0x7b450ffd277c963fU, 0x7f4e47323bb2c5cfU, 0x57553ae60b3177a9U, 0xd46a3d74b9df1a4eU,
  0xe7f01125bdbb0841U, 0xd4dd8ebf0121d46U, 0xc5441a1403c6b3d9U, 0x2f8113b809c52a8eU,
  0x63bbcca1a62172c7U, 0xbc6c08a2cae0461cU, 0x4fef86137deff5feU, 0x40dde54bce55c481U,
  0x3b96eb5ff965342dU, 0x1b7f36791332a8bbU, 0xadecf0e157d54904U, 0xb43d44d59c4548b2U,
  0xc334c1babcbc1a92U, 0x3d3ab46936632ddbU, 0x9c4881aa9b8191a5U, 0xc1a25677b200b756U,
  0xfffff61c00001751U, 0xfffffd0e00000d70U, 0xffffec4efffffbb9U, 0xfffffee90000183aU,
  0xfffff3f5fffffed7U, 0x13d000001039U, 0x381U, 0x141affffff1fU,
  0xb6dffffe939U, 0x1828U, 0xfffffe1bfffff752U, 0xfffffd1a00000000U,
  0x2599ffffee1dU, 0xfffff1d500000000U, 0xfffff46600001054U, 0xfffffd5bU,
  0x2a93d4be8c870646U, 0x41f6c78fb36b26d1U, 0x89ec7fa453bdab94U, 0x23cd63bd1570e6f6U,
  0xd8810325e24068e9U, 0xf4580007132d2234U, 0x5d1a00e114556445U, 0xcac6ab570a2bdc9fU,
  0xb517aae04ce9f154U, 0x3c4cbd2153d0f40fU, 0xe24cc2ca48f032b3U, 0xd005dcdaf2007f3bU,
  0x17cf3338eed3f47fU, 0xe539cdeec4d4323eU, 0x3ebde42cd92fdacdU, 0xc1bdd93ac6002dc2U,
  0xbb36336fe3a21d9cU, 0xbba6b202c09129dU, 0x10596fd359819bdbU, 0x157308a3c9b1d1bU,
  0xede8192ed0b044d2U, 0xa73d400c81cc4602U, 0xdb26f146d1e7dd20U, 0xbcc040060c23d746U,
  0x21e534d8d37f0acfU, 0xfa1e21ec7bcf16ceU, 0x2c00070a1e05d512U, 0xe28e1e8f7f9d92aU,
  0x3fe011d7bb7f20a5U, 0x54d8be41341245faU, 0x3148b8bfd8e2f1f7U, 0x3c242152ddf5c3c2U,
  0x26e5121c2dc92221U, 0xebcccdfb81cafc09U, 0x321ae33ebefc121fU, 0xf6d31dfc2631c7ffU,
  0x37acc8ea40765181U, 0x630f30450fee90cU, 0xec526b5962881436U, 0xc5d96ae85b342a49U,
  0x1d46d3f34130b33eU, 0x44f0c21d2229c00dU, 0x27bff0cbfd2ce0ccU, 0xbaff2652bacc7f07U,
  0xae7fc1dd525fae34U, 0x57c9b827064a49ceU, 0xf40e02bdb61a32afU, 0x1be6b52db9f70249U,
  0xe61b3d1ce7a41ccaU, 0xb2a190181f7e5d0U, 0x36c70cd7c7fdf53dU, 0xccecc4fc36e6feecU,
  0x3b35d66e4eda3029U, 0x2e4497919eba85a6U, 0x1842a8c33a811fd8U, 0x691151abff1f0c2bU,
  0xd44a22c030904df5U, 0x81ac40f4dce6ef3eU, 0x4cf2d0d421241627U, 0x85524d4191abf32U,
  0xf8e12ad305ac2aecU, 0xcd36fcc0813f1ebeU, 0xf9ee06eef2fecf37U, 0x33eb0f343e21e1daU,
  0x19000000000U, 0xfffffede0000112cU, 0xfffff9bffffff70bU, 0x2cdffffff1eU,
  0x42efffff978U, 0x10e20000126dU, 0xfffff680fffff909U, 0xfffff9a9fffff793U,
  0xf1997a5a247cc300U, 0x3238b15d7b7fc928U, 0xfffffe47U,
};


ai_handle g_hello_world_weights_table[1 + 2] = {
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
  AI_HANDLE_PTR(s_hello_world_weights_array_u64),
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
};

