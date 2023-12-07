
build/prog/enclave/musl_eapp/eapp_thread:     file format elf64-littleriscv


Disassembly of section .text:

0000000000010120 <exit>:
   10120:	1101                	addi	sp,sp,-32
   10122:	e42a                	sd	a0,8(sp)
   10124:	ec06                	sd	ra,24(sp)
   10126:	2e6000ef          	jal	ra,1040c <__funcs_on_exit>
   1012a:	2e4000ef          	jal	ra,1040e <__libc_exit_fini>
   1012e:	2c6030ef          	jal	ra,133f4 <__stdio_exit>
   10132:	6522                	ld	a0,8(sp)
   10134:	458020ef          	jal	ra,1258c <_Exit>

0000000000010138 <main>:
   10138:	1101                	addi	sp,sp,-32
   1013a:	6641                	lui	a2,0x10
   1013c:	4685                	li	a3,1
   1013e:	23a60613          	addi	a2,a2,570 # 1023a <thread>
   10142:	4581                	li	a1,0
   10144:	0028                	addi	a0,sp,8
   10146:	ec06                	sd	ra,24(sp)
   10148:	3d3010ef          	jal	ra,11d1a <__pthread_create>
   1014c:	6522                	ld	a0,8(sp)
   1014e:	4581                	li	a1,0
   10150:	0c4020ef          	jal	ra,12214 <__pthread_join>
   10154:	60e2                	ld	ra,24(sp)
   10156:	4501                	li	a0,0
   10158:	6105                	addi	sp,sp,32
   1015a:	8082                	ret

000000000001015c <_start>:
   1015c:	00008197          	auipc	gp,0x8
   10160:	6a418193          	addi	gp,gp,1700 # 18800 <__global_pointer$>
   10164:	850a                	mv	a0,sp
   10166:	ffff0597          	auipc	a1,0xffff0
   1016a:	e9a58593          	addi	a1,a1,-358 # 0 <exit-0x10120>
   1016e:	ff017113          	andi	sp,sp,-16
   10172:	a009                	j	10174 <_start_c>

0000000000010174 <_start_c>:
   10174:	410c                	lw	a1,0(a0)
   10176:	6741                	lui	a4,0x10
   10178:	66c1                	lui	a3,0x10
   1017a:	00850613          	addi	a2,a0,8
   1017e:	6541                	lui	a0,0x10
   10180:	4781                	li	a5,0
   10182:	40c70713          	addi	a4,a4,1036 # 1040c <__funcs_on_exit>
   10186:	25868693          	addi	a3,a3,600 # 10258 <_init>
   1018a:	13850513          	addi	a0,a0,312 # 10138 <main>
   1018e:	a4b1                	j	103da <__libc_start_main>

0000000000010190 <deregister_tm_clones>:
   10190:	8e818793          	addi	a5,gp,-1816 # 180e8 <__TMC_END__>
   10194:	8e818713          	addi	a4,gp,-1816 # 180e8 <__TMC_END__>
   10198:	00f70b63          	beq	a4,a5,101ae <deregister_tm_clones+0x1e>
   1019c:	00000337          	lui	t1,0x0
   101a0:	00030313          	mv	t1,t1
   101a4:	00030563          	beqz	t1,101ae <deregister_tm_clones+0x1e>
   101a8:	8e818513          	addi	a0,gp,-1816 # 180e8 <__TMC_END__>
   101ac:	8302                	jr	t1
   101ae:	8082                	ret

00000000000101b0 <register_tm_clones>:
   101b0:	8e818593          	addi	a1,gp,-1816 # 180e8 <__TMC_END__>
   101b4:	8e818793          	addi	a5,gp,-1816 # 180e8 <__TMC_END__>
   101b8:	8f8d                	sub	a5,a5,a1
   101ba:	4037d713          	srai	a4,a5,0x3
   101be:	03f7d593          	srli	a1,a5,0x3f
   101c2:	95ba                	add	a1,a1,a4
   101c4:	8585                	srai	a1,a1,0x1
   101c6:	c991                	beqz	a1,101da <register_tm_clones+0x2a>
   101c8:	00000337          	lui	t1,0x0
   101cc:	00030313          	mv	t1,t1
   101d0:	00030563          	beqz	t1,101da <register_tm_clones+0x2a>
   101d4:	8e818513          	addi	a0,gp,-1816 # 180e8 <__TMC_END__>
   101d8:	8302                	jr	t1
   101da:	8082                	ret

00000000000101dc <__do_global_dtors_aux>:
   101dc:	1141                	addi	sp,sp,-16
   101de:	e022                	sd	s0,0(sp)
   101e0:	9901c783          	lbu	a5,-1648(gp) # 18190 <completed.4703>
   101e4:	e406                	sd	ra,8(sp)
   101e6:	e395                	bnez	a5,1020a <__do_global_dtors_aux+0x2e>
   101e8:	fa9ff0ef          	jal	ra,10190 <deregister_tm_clones>
   101ec:	000007b7          	lui	a5,0x0
   101f0:	00078793          	mv	a5,a5
   101f4:	cb81                	beqz	a5,10204 <__do_global_dtors_aux+0x28>
   101f6:	6559                	lui	a0,0x16
   101f8:	3a850513          	addi	a0,a0,936 # 163a8 <__FRAME_END__>
   101fc:	00000097          	auipc	ra,0x0
   10200:	000000e7          	jalr	zero # 0 <exit-0x10120>
   10204:	4785                	li	a5,1
   10206:	98f18823          	sb	a5,-1648(gp) # 18190 <completed.4703>
   1020a:	60a2                	ld	ra,8(sp)
   1020c:	6402                	ld	s0,0(sp)
   1020e:	0141                	addi	sp,sp,16
   10210:	8082                	ret

0000000000010212 <frame_dummy>:
   10212:	000007b7          	lui	a5,0x0
   10216:	00078793          	mv	a5,a5
   1021a:	cf99                	beqz	a5,10238 <frame_dummy+0x26>
   1021c:	6559                	lui	a0,0x16
   1021e:	1141                	addi	sp,sp,-16
   10220:	99818593          	addi	a1,gp,-1640 # 18198 <object.4708>
   10224:	3a850513          	addi	a0,a0,936 # 163a8 <__FRAME_END__>
   10228:	e406                	sd	ra,8(sp)
   1022a:	00000097          	auipc	ra,0x0
   1022e:	000000e7          	jalr	zero # 0 <exit-0x10120>
   10232:	60a2                	ld	ra,8(sp)
   10234:	0141                	addi	sp,sp,16
   10236:	bfad                	j	101b0 <register_tm_clones>
   10238:	bfa5                	j	101b0 <register_tm_clones>

000000000001023a <thread>:
   1023a:	1141                	addi	sp,sp,-16
   1023c:	e022                	sd	s0,0(sp)
   1023e:	85aa                	mv	a1,a0
   10240:	842a                	mv	s0,a0
   10242:	6555                	lui	a0,0x15
   10244:	3b050513          	addi	a0,a0,944 # 153b0 <__clzdi2+0x4e>
   10248:	e406                	sd	ra,8(sp)
   1024a:	1f0000ef          	jal	ra,1043a <printf>
   1024e:	60a2                	ld	ra,8(sp)
   10250:	8522                	mv	a0,s0
   10252:	6402                	ld	s0,0(sp)
   10254:	0141                	addi	sp,sp,16
   10256:	8082                	ret

0000000000010258 <_init>:
   10258:	8082                	ret

000000000001025a <__init_ssp>:
   1025a:	8082                	ret

000000000001025c <__init_libc>:
   1025c:	7109                	addi	sp,sp,-384
   1025e:	f2ca                	sd	s2,352(sp)
   10260:	03010913          	addi	s2,sp,48
   10264:	faa2                	sd	s0,368(sp)
   10266:	f6a6                	sd	s1,360(sp)
   10268:	842e                	mv	s0,a1
   1026a:	84aa                	mv	s1,a0
   1026c:	13000613          	li	a2,304
   10270:	4581                	li	a1,0
   10272:	854a                	mv	a0,s2
   10274:	fe86                	sd	ra,376(sp)
   10276:	6a2010ef          	jal	ra,11918 <memset>
   1027a:	9691b823          	sd	s1,-1680(gp) # 18170 <__environ>
   1027e:	4501                	li	a0,0
   10280:	00351793          	slli	a5,a0,0x3
   10284:	97a6                	add	a5,a5,s1
   10286:	639c                	ld	a5,0(a5)
   10288:	0505                	addi	a0,a0,1
   1028a:	fbfd                	bnez	a5,10280 <__init_libc+0x24>
   1028c:	050e                	slli	a0,a0,0x3
   1028e:	9526                	add	a0,a0,s1
   10290:	f6018793          	addi	a5,gp,-160 # 18760 <__libc>
   10294:	e788                	sd	a0,8(a5)
   10296:	f6018493          	addi	s1,gp,-160 # 18760 <__libc>
   1029a:	02500713          	li	a4,37
   1029e:	611c                	ld	a5,0(a0)
   102a0:	e7d5                	bnez	a5,1034c <__init_libc+0xf0>
   102a2:	774a                	ld	a4,176(sp)
   102a4:	f4e1bc23          	sd	a4,-168(gp) # 18758 <__hwcap>
   102a8:	77d2                	ld	a5,304(sp)
   102aa:	c399                	beqz	a5,102b0 <__init_libc+0x54>
   102ac:	f4f1b823          	sd	a5,-176(gp) # 18750 <__sysinfo>
   102b0:	6785                	lui	a5,0x1
   102b2:	f89c                	sd	a5,48(s1)
   102b4:	e411                	bnez	s0,102c0 <__init_libc+0x64>
   102b6:	7432                	ld	s0,296(sp)
   102b8:	e401                	bnez	s0,102c0 <__init_libc+0x64>
   102ba:	6459                	lui	s0,0x16
   102bc:	91840413          	addi	s0,s0,-1768 # 15918 <__func__.3458+0x40>
   102c0:	9481b023          	sd	s0,-1728(gp) # 18140 <__progname_full>
   102c4:	9281bc23          	sd	s0,-1736(gp) # 18138 <__progname>
   102c8:	02f00693          	li	a3,47
   102cc:	0405                	addi	s0,s0,1
   102ce:	fff44703          	lbu	a4,-1(s0)
   102d2:	e759                	bnez	a4,10360 <__init_libc+0x104>
   102d4:	854a                	mv	a0,s2
   102d6:	142020ef          	jal	ra,12418 <__init_tls>
   102da:	756e                	ld	a0,248(sp)
   102dc:	f7fff0ef          	jal	ra,1025a <__init_ssp>
   102e0:	672a                	ld	a4,136(sp)
   102e2:	67ca                	ld	a5,144(sp)
   102e4:	00f71863          	bne	a4,a5,102f4 <__init_libc+0x98>
   102e8:	676a                	ld	a4,152(sp)
   102ea:	778a                	ld	a5,160(sp)
   102ec:	00f71463          	bne	a4,a5,102f4 <__init_libc+0x98>
   102f0:	77ae                	ld	a5,232(sp)
   102f2:	c3d9                	beqz	a5,10378 <__init_libc+0x11c>
   102f4:	f002                	sd	zero,32(sp)
   102f6:	4785                	li	a5,1
   102f8:	f402                	sd	zero,40(sp)
   102fa:	d03e                	sw	a5,32(sp)
   102fc:	4789                	li	a5,2
   102fe:	ec02                	sd	zero,24(sp)
   10300:	d43e                	sw	a5,40(sp)
   10302:	e402                	sd	zero,8(sp)
   10304:	e802                	sd	zero,16(sp)
   10306:	04900893          	li	a7,73
   1030a:	0828                	addi	a0,sp,24
   1030c:	458d                	li	a1,3
   1030e:	0030                	addi	a2,sp,8
   10310:	4681                	li	a3,0
   10312:	4721                	li	a4,8
   10314:	00000073          	ecall
   10318:	02051713          	slli	a4,a0,0x20
   1031c:	083c                	addi	a5,sp,24
   1031e:	02074463          	bltz	a4,10346 <__init_libc+0xea>
   10322:	6621                	lui	a2,0x8
   10324:	66d5                	lui	a3,0x15
   10326:	0609                	addi	a2,a2,2
   10328:	0067d703          	lhu	a4,6(a5) # 1006 <exit-0xf11a>
   1032c:	02077713          	andi	a4,a4,32
   10330:	cf15                	beqz	a4,1036c <__init_libc+0x110>
   10332:	03800893          	li	a7,56
   10336:	f9c00513          	li	a0,-100
   1033a:	3c068593          	addi	a1,a3,960 # 153c0 <__clzdi2+0x5e>
   1033e:	00000073          	ecall
   10342:	02055563          	bgez	a0,1036c <__init_libc+0x110>
   10346:	00000023          	sb	zero,0(zero) # 0 <exit-0x10120>
   1034a:	9002                	ebreak
   1034c:	00f76863          	bltu	a4,a5,1035c <__init_libc+0x100>
   10350:	1294                	addi	a3,sp,352
   10352:	078e                	slli	a5,a5,0x3
   10354:	97b6                	add	a5,a5,a3
   10356:	6514                	ld	a3,8(a0)
   10358:	ecd7b823          	sd	a3,-304(a5)
   1035c:	0541                	addi	a0,a0,16
   1035e:	b781                	j	1029e <__init_libc+0x42>
   10360:	00d71463          	bne	a4,a3,10368 <__init_libc+0x10c>
   10364:	9281bc23          	sd	s0,-1736(gp) # 18138 <__progname>
   10368:	0405                	addi	s0,s0,1
   1036a:	b795                	j	102ce <__init_libc+0x72>
   1036c:	07a1                	addi	a5,a5,8
   1036e:	faf91de3          	bne	s2,a5,10328 <__init_libc+0xcc>
   10372:	4785                	li	a5,1
   10374:	00f48123          	sb	a5,2(s1)
   10378:	70f6                	ld	ra,376(sp)
   1037a:	7456                	ld	s0,368(sp)
   1037c:	74b6                	ld	s1,360(sp)
   1037e:	7916                	ld	s2,352(sp)
   10380:	6119                	addi	sp,sp,384
   10382:	8082                	ret

0000000000010384 <__libc_start_init>:
   10384:	1101                	addi	sp,sp,-32
   10386:	e822                	sd	s0,16(sp)
   10388:	e426                	sd	s1,8(sp)
   1038a:	6461                	lui	s0,0x18
   1038c:	64e1                	lui	s1,0x18
   1038e:	ec06                	sd	ra,24(sp)
   10390:	ff040413          	addi	s0,s0,-16 # 17ff0 <__frame_dummy_init_array_entry>
   10394:	ec5ff0ef          	jal	ra,10258 <_init>
   10398:	ff848493          	addi	s1,s1,-8 # 17ff8 <__do_global_dtors_aux_fini_array_entry>
   1039c:	00946763          	bltu	s0,s1,103aa <__libc_start_init+0x26>
   103a0:	60e2                	ld	ra,24(sp)
   103a2:	6442                	ld	s0,16(sp)
   103a4:	64a2                	ld	s1,8(sp)
   103a6:	6105                	addi	sp,sp,32
   103a8:	8082                	ret
   103aa:	601c                	ld	a5,0(s0)
   103ac:	0421                	addi	s0,s0,8
   103ae:	9782                	jalr	a5
   103b0:	b7f5                	j	1039c <__libc_start_init+0x18>

00000000000103b2 <libc_start_main_stage2>:
   103b2:	7179                	addi	sp,sp,-48
   103b4:	f022                	sd	s0,32(sp)
   103b6:	00158413          	addi	s0,a1,1
   103ba:	040e                	slli	s0,s0,0x3
   103bc:	f406                	sd	ra,40(sp)
   103be:	ec26                	sd	s1,24(sp)
   103c0:	e84a                	sd	s2,16(sp)
   103c2:	84ae                	mv	s1,a1
   103c4:	892a                	mv	s2,a0
   103c6:	9432                	add	s0,s0,a2
   103c8:	e432                	sd	a2,8(sp)
   103ca:	fbbff0ef          	jal	ra,10384 <__libc_start_init>
   103ce:	65a2                	ld	a1,8(sp)
   103d0:	8622                	mv	a2,s0
   103d2:	8526                	mv	a0,s1
   103d4:	9902                	jalr	s2
   103d6:	d4bff0ef          	jal	ra,10120 <exit>

00000000000103da <__libc_start_main>:
   103da:	7179                	addi	sp,sp,-48
   103dc:	f022                	sd	s0,32(sp)
   103de:	ec26                	sd	s1,24(sp)
   103e0:	842e                	mv	s0,a1
   103e2:	84aa                	mv	s1,a0
   103e4:	00158513          	addi	a0,a1,1
   103e8:	620c                	ld	a1,0(a2)
   103ea:	050e                	slli	a0,a0,0x3
   103ec:	9532                	add	a0,a0,a2
   103ee:	f406                	sd	ra,40(sp)
   103f0:	e432                	sd	a2,8(sp)
   103f2:	e6bff0ef          	jal	ra,1025c <__init_libc>
   103f6:	6341                	lui	t1,0x10
   103f8:	3b230313          	addi	t1,t1,946 # 103b2 <libc_start_main_stage2>
   103fc:	85a2                	mv	a1,s0
   103fe:	7402                	ld	s0,32(sp)
   10400:	6622                	ld	a2,8(sp)
   10402:	70a2                	ld	ra,40(sp)
   10404:	8526                	mv	a0,s1
   10406:	64e2                	ld	s1,24(sp)
   10408:	6145                	addi	sp,sp,48
   1040a:	8302                	jr	t1

000000000001040c <__funcs_on_exit>:
   1040c:	8082                	ret

000000000001040e <__libc_exit_fini>:
   1040e:	1101                	addi	sp,sp,-32
   10410:	e822                	sd	s0,16(sp)
   10412:	e426                	sd	s1,8(sp)
   10414:	6461                	lui	s0,0x18
   10416:	64e1                	lui	s1,0x18
   10418:	ec06                	sd	ra,24(sp)
   1041a:	00040413          	mv	s0,s0
   1041e:	ff848493          	addi	s1,s1,-8 # 17ff8 <__do_global_dtors_aux_fini_array_entry>
   10422:	0084e763          	bltu	s1,s0,10430 <__libc_exit_fini+0x22>
   10426:	6442                	ld	s0,16(sp)
   10428:	60e2                	ld	ra,24(sp)
   1042a:	64a2                	ld	s1,8(sp)
   1042c:	6105                	addi	sp,sp,32
   1042e:	bff9                	j	1040c <__funcs_on_exit>
   10430:	ff843783          	ld	a5,-8(s0) # 17ff8 <__do_global_dtors_aux_fini_array_entry>
   10434:	1461                	addi	s0,s0,-8
   10436:	9782                	jalr	a5
   10438:	b7ed                	j	10422 <__libc_exit_fini+0x14>

000000000001043a <printf>:
   1043a:	711d                	addi	sp,sp,-96
   1043c:	f42e                	sd	a1,40(sp)
   1043e:	85aa                	mv	a1,a0
   10440:	6561                	lui	a0,0x18
   10442:	f832                	sd	a2,48(sp)
   10444:	00050513          	mv	a0,a0
   10448:	1030                	addi	a2,sp,40
   1044a:	ec06                	sd	ra,24(sp)
   1044c:	fc36                	sd	a3,56(sp)
   1044e:	e0ba                	sd	a4,64(sp)
   10450:	e4be                	sd	a5,72(sp)
   10452:	e8c2                	sd	a6,80(sp)
   10454:	ecc6                	sd	a7,88(sp)
   10456:	e432                	sd	a2,8(sp)
   10458:	3c6010ef          	jal	ra,1181e <vfprintf>
   1045c:	60e2                	ld	ra,24(sp)
   1045e:	6125                	addi	sp,sp,96
   10460:	8082                	ret

0000000000010462 <pop_arg>:
   10462:	35dd                	addiw	a1,a1,-9
   10464:	0005871b          	sext.w	a4,a1
   10468:	47c5                	li	a5,17
   1046a:	08e7ee63          	bltu	a5,a4,10506 <pop_arg+0xa4>
   1046e:	1582                	slli	a1,a1,0x20
   10470:	67d5                	lui	a5,0x15
   10472:	3cc78793          	addi	a5,a5,972 # 153cc <__clzdi2+0x6a>
   10476:	81f9                	srli	a1,a1,0x1e
   10478:	95be                	add	a1,a1,a5
   1047a:	4198                	lw	a4,0(a1)
   1047c:	1141                	addi	sp,sp,-16
   1047e:	e022                	sd	s0,0(sp)
   10480:	621c                	ld	a5,0(a2)
   10482:	e406                	sd	ra,8(sp)
   10484:	842a                	mv	s0,a0
   10486:	8702                	jr	a4
   10488:	00878713          	addi	a4,a5,8
   1048c:	439c                	lw	a5,0(a5)
   1048e:	e218                	sd	a4,0(a2)
   10490:	e01c                	sd	a5,0(s0)
   10492:	60a2                	ld	ra,8(sp)
   10494:	6402                	ld	s0,0(sp)
   10496:	0141                	addi	sp,sp,16
   10498:	8082                	ret
   1049a:	00878713          	addi	a4,a5,8
   1049e:	e218                	sd	a4,0(a2)
   104a0:	0007e783          	lwu	a5,0(a5)
   104a4:	b7f5                	j	10490 <pop_arg+0x2e>
   104a6:	00878713          	addi	a4,a5,8
   104aa:	e218                	sd	a4,0(a2)
   104ac:	00079783          	lh	a5,0(a5)
   104b0:	b7c5                	j	10490 <pop_arg+0x2e>
   104b2:	00878713          	addi	a4,a5,8
   104b6:	e218                	sd	a4,0(a2)
   104b8:	0007d783          	lhu	a5,0(a5)
   104bc:	bfd1                	j	10490 <pop_arg+0x2e>
   104be:	00878713          	addi	a4,a5,8
   104c2:	e218                	sd	a4,0(a2)
   104c4:	00078783          	lb	a5,0(a5)
   104c8:	b7e1                	j	10490 <pop_arg+0x2e>
   104ca:	00878713          	addi	a4,a5,8
   104ce:	e218                	sd	a4,0(a2)
   104d0:	0007c783          	lbu	a5,0(a5)
   104d4:	bf75                	j	10490 <pop_arg+0x2e>
   104d6:	00878713          	addi	a4,a5,8
   104da:	e218                	sd	a4,0(a2)
   104dc:	639c                	ld	a5,0(a5)
   104de:	bf4d                	j	10490 <pop_arg+0x2e>
   104e0:	2388                	fld	fa0,0(a5)
   104e2:	00878713          	addi	a4,a5,8
   104e6:	e218                	sd	a4,0(a2)
   104e8:	585040ef          	jal	ra,1526c <__extenddftf2>
   104ec:	e008                	sd	a0,0(s0)
   104ee:	e40c                	sd	a1,8(s0)
   104f0:	b74d                	j	10492 <pop_arg+0x30>
   104f2:	07bd                	addi	a5,a5,15
   104f4:	9bc1                	andi	a5,a5,-16
   104f6:	01078713          	addi	a4,a5,16
   104fa:	e218                	sd	a4,0(a2)
   104fc:	6398                	ld	a4,0(a5)
   104fe:	679c                	ld	a5,8(a5)
   10500:	e118                	sd	a4,0(a0)
   10502:	e51c                	sd	a5,8(a0)
   10504:	b779                	j	10492 <pop_arg+0x30>
   10506:	8082                	ret

0000000000010508 <fmt_u>:
   10508:	87aa                	mv	a5,a0
   1050a:	46a9                	li	a3,10
   1050c:	852e                	mv	a0,a1
   1050e:	e391                	bnez	a5,10512 <fmt_u+0xa>
   10510:	8082                	ret
   10512:	02d7f733          	remu	a4,a5,a3
   10516:	157d                	addi	a0,a0,-1
   10518:	0307071b          	addiw	a4,a4,48
   1051c:	02d7d7b3          	divu	a5,a5,a3
   10520:	00e50023          	sb	a4,0(a0) # 18000 <__stdout_FILE>
   10524:	b7ed                	j	1050e <fmt_u+0x6>

0000000000010526 <getint>:
   10526:	0cccd5b7          	lui	a1,0xcccd
   1052a:	80000837          	lui	a6,0x80000
   1052e:	862a                	mv	a2,a0
   10530:	4325                	li	t1,9
   10532:	4501                	li	a0,0
   10534:	ccc58593          	addi	a1,a1,-820 # ccccccc <__global_pointer$+0xccb44cc>
   10538:	5e59                	li	t3,-10
   1053a:	fff84813          	not	a6,a6
   1053e:	4ea9                	li	t4,10
   10540:	6214                	ld	a3,0(a2)
   10542:	0006c703          	lbu	a4,0(a3)
   10546:	fd07089b          	addiw	a7,a4,-48
   1054a:	8746                	mv	a4,a7
   1054c:	01137363          	bgeu	t1,a7,10552 <getint+0x2c>
   10550:	8082                	ret
   10552:	00a5ef63          	bltu	a1,a0,10570 <getint+0x4a>
   10556:	02ae07bb          	mulw	a5,t3,a0
   1055a:	010787bb          	addw	a5,a5,a6
   1055e:	0117c963          	blt	a5,a7,10570 <getint+0x4a>
   10562:	02ae87bb          	mulw	a5,t4,a0
   10566:	00f7053b          	addw	a0,a4,a5
   1056a:	0685                	addi	a3,a3,1
   1056c:	e214                	sd	a3,0(a2)
   1056e:	bfc9                	j	10540 <getint+0x1a>
   10570:	557d                	li	a0,-1
   10572:	bfe5                	j	1056a <getint+0x44>

0000000000010574 <out>:
   10574:	87aa                	mv	a5,a0
   10576:	4398                	lw	a4,0(a5)
   10578:	852e                	mv	a0,a1
   1057a:	85b2                	mv	a1,a2
   1057c:	02077713          	andi	a4,a4,32
   10580:	e701                	bnez	a4,10588 <out+0x14>
   10582:	863e                	mv	a2,a5
   10584:	3980206f          	j	1291c <__fwritex>
   10588:	8082                	ret

000000000001058a <pad>:
   1058a:	67c9                	lui	a5,0x12
   1058c:	8f7d                	and	a4,a4,a5
   1058e:	e73d                	bnez	a4,105fc <pad+0x72>
   10590:	06c6d663          	bge	a3,a2,105fc <pad+0x72>
   10594:	7169                	addi	sp,sp,-304
   10596:	f222                	sd	s0,288(sp)
   10598:	ee26                	sd	s1,280(sp)
   1059a:	ea4a                	sd	s2,272(sp)
   1059c:	40d604bb          	subw	s1,a2,a3
   105a0:	f606                	sd	ra,296(sp)
   105a2:	e64e                	sd	s3,264(sp)
   105a4:	10000793          	li	a5,256
   105a8:	892a                	mv	s2,a0
   105aa:	8426                	mv	s0,s1
   105ac:	0004861b          	sext.w	a2,s1
   105b0:	0097d463          	bge	a5,s1,105b8 <pad+0x2e>
   105b4:	10000613          	li	a2,256
   105b8:	850a                	mv	a0,sp
   105ba:	35e010ef          	jal	ra,11918 <memset>
   105be:	0ff00993          	li	s3,255
   105c2:	0299c463          	blt	s3,s1,105ea <pad+0x60>
   105c6:	0084561b          	srliw	a2,s0,0x8
   105ca:	f0000793          	li	a5,-256
   105ce:	02f6063b          	mulw	a2,a2,a5
   105d2:	858a                	mv	a1,sp
   105d4:	854a                	mv	a0,s2
   105d6:	9e21                	addw	a2,a2,s0
   105d8:	f9dff0ef          	jal	ra,10574 <out>
   105dc:	70b2                	ld	ra,296(sp)
   105de:	7412                	ld	s0,288(sp)
   105e0:	64f2                	ld	s1,280(sp)
   105e2:	6952                	ld	s2,272(sp)
   105e4:	69b2                	ld	s3,264(sp)
   105e6:	6155                	addi	sp,sp,304
   105e8:	8082                	ret
   105ea:	10000613          	li	a2,256
   105ee:	858a                	mv	a1,sp
   105f0:	854a                	mv	a0,s2
   105f2:	f83ff0ef          	jal	ra,10574 <out>
   105f6:	f004849b          	addiw	s1,s1,-256
   105fa:	b7e1                	j	105c2 <pad+0x38>
   105fc:	8082                	ret

00000000000105fe <fmt_fp>:
   105fe:	72f9                	lui	t0,0xffffe
   10600:	7119                	addi	sp,sp,-128
   10602:	2c028293          	addi	t0,t0,704 # ffffffffffffe2c0 <__global_pointer$+0xfffffffffffe5ac0>
   10606:	f8a2                	sd	s0,112(sp)
   10608:	f4a6                	sd	s1,104(sp)
   1060a:	ecce                	sd	s3,88(sp)
   1060c:	e8d2                	sd	s4,80(sp)
   1060e:	e4d6                	sd	s5,72(sp)
   10610:	fc5e                	sd	s7,56(sp)
   10612:	fc86                	sd	ra,120(sp)
   10614:	f0ca                	sd	s2,96(sp)
   10616:	e0da                	sd	s6,64(sp)
   10618:	f862                	sd	s8,48(sp)
   1061a:	f466                	sd	s9,40(sp)
   1061c:	f06a                	sd	s10,32(sp)
   1061e:	ec6e                	sd	s11,24(sp)
   10620:	843a                	mv	s0,a4
   10622:	9116                	add	sp,sp,t0
   10624:	6709                	lui	a4,0x2
   10626:	e036                	sd	a3,0(sp)
   10628:	d1070713          	addi	a4,a4,-752 # 1d10 <exit-0xe410>
   1062c:	0094                	addi	a3,sp,64
   1062e:	8bbe                	mv	s7,a5
   10630:	9736                	add	a4,a4,a3
   10632:	77f9                	lui	a5,0xffffe
   10634:	97ba                	add	a5,a5,a4
   10636:	84aa                	mv	s1,a0
   10638:	8aae                	mv	s5,a1
   1063a:	852e                	mv	a0,a1
   1063c:	85b2                	mv	a1,a2
   1063e:	8a32                	mv	s4,a2
   10640:	89c2                	mv	s3,a6
   10642:	e83e                	sd	a5,16(sp)
   10644:	2e07ae23          	sw	zero,764(a5) # ffffffffffffe2fc <__global_pointer$+0xfffffffffffe5afc>
   10648:	78f010ef          	jal	ra,125d6 <__signbitl>
   1064c:	cd49                	beqz	a0,106e6 <fmt_fp+0xe8>
   1064e:	57fd                	li	a5,-1
   10650:	17fe                	slli	a5,a5,0x3f
   10652:	6955                	lui	s2,0x15
   10654:	00fa4a33          	xor	s4,s4,a5
   10658:	4c05                	li	s8,1
   1065a:	41890913          	addi	s2,s2,1048 # 15418 <__clzdi2+0xb6>
   1065e:	8556                	mv	a0,s5
   10660:	85d2                	mv	a1,s4
   10662:	74b010ef          	jal	ra,125ac <__fpclassifyl>
   10666:	4785                	li	a5,1
   10668:	0aa7ce63          	blt	a5,a0,10724 <fmt_fp+0x126>
   1066c:	0209f993          	andi	s3,s3,32
   10670:	00098c9b          	sext.w	s9,s3
   10674:	0a099063          	bnez	s3,10714 <fmt_fp+0x116>
   10678:	6455                	lui	s0,0x15
   1067a:	43840413          	addi	s0,s0,1080 # 15438 <__clzdi2+0xd6>
   1067e:	8656                	mv	a2,s5
   10680:	86d2                	mv	a3,s4
   10682:	8556                	mv	a0,s5
   10684:	85d2                	mv	a1,s4
   10686:	05f030ef          	jal	ra,13ee4 <__eqtf2>
   1068a:	c511                	beqz	a0,10696 <fmt_fp+0x98>
   1068c:	080c9863          	bnez	s9,1071c <fmt_fp+0x11e>
   10690:	6455                	lui	s0,0x15
   10692:	44840413          	addi	s0,s0,1096 # 15448 <__clzdi2+0xe6>
   10696:	6602                	ld	a2,0(sp)
   10698:	7741                	lui	a4,0xffff0
   1069a:	003c0a1b          	addiw	s4,s8,3
   1069e:	177d                	addi	a4,a4,-1
   106a0:	00ebf733          	and	a4,s7,a4
   106a4:	86d2                	mv	a3,s4
   106a6:	02000593          	li	a1,32
   106aa:	8526                	mv	a0,s1
   106ac:	edfff0ef          	jal	ra,1058a <pad>
   106b0:	8662                	mv	a2,s8
   106b2:	85ca                	mv	a1,s2
   106b4:	8526                	mv	a0,s1
   106b6:	ebfff0ef          	jal	ra,10574 <out>
   106ba:	460d                	li	a2,3
   106bc:	85a2                	mv	a1,s0
   106be:	8526                	mv	a0,s1
   106c0:	eb5ff0ef          	jal	ra,10574 <out>
   106c4:	6602                	ld	a2,0(sp)
   106c6:	6709                	lui	a4,0x2
   106c8:	00ebc733          	xor	a4,s7,a4
   106cc:	86d2                	mv	a3,s4
   106ce:	02000593          	li	a1,32
   106d2:	8526                	mv	a0,s1
   106d4:	eb7ff0ef          	jal	ra,1058a <pad>
   106d8:	6782                	ld	a5,0(sp)
   106da:	89d2                	mv	s3,s4
   106dc:	1cfa55e3          	bge	s4,a5,110a6 <fmt_fp+0xaa8>
   106e0:	4982                	lw	s3,0(sp)
   106e2:	1c50006f          	j	110a6 <fmt_fp+0xaa8>
   106e6:	034b9713          	slli	a4,s7,0x34
   106ea:	000b879b          	sext.w	a5,s7
   106ee:	00074963          	bltz	a4,10700 <fmt_fp+0x102>
   106f2:	8b85                	andi	a5,a5,1
   106f4:	eb99                	bnez	a5,1070a <fmt_fp+0x10c>
   106f6:	6955                	lui	s2,0x15
   106f8:	8c2a                	mv	s8,a0
   106fa:	41990913          	addi	s2,s2,1049 # 15419 <__clzdi2+0xb7>
   106fe:	b785                	j	1065e <fmt_fp+0x60>
   10700:	6955                	lui	s2,0x15
   10702:	4c05                	li	s8,1
   10704:	41b90913          	addi	s2,s2,1051 # 1541b <__clzdi2+0xb9>
   10708:	bf99                	j	1065e <fmt_fp+0x60>
   1070a:	6955                	lui	s2,0x15
   1070c:	4c05                	li	s8,1
   1070e:	41e90913          	addi	s2,s2,1054 # 1541e <__clzdi2+0xbc>
   10712:	b7b1                	j	1065e <fmt_fp+0x60>
   10714:	6455                	lui	s0,0x15
   10716:	43040413          	addi	s0,s0,1072 # 15430 <__clzdi2+0xce>
   1071a:	b795                	j	1067e <fmt_fp+0x80>
   1071c:	6455                	lui	s0,0x15
   1071e:	44040413          	addi	s0,s0,1088 # 15440 <__clzdi2+0xde>
   10722:	bf95                	j	10696 <fmt_fp+0x98>
   10724:	6c89                	lui	s9,0x2
   10726:	0098                	addi	a4,sp,64
   10728:	7d79                	lui	s10,0xffffe
   1072a:	d10c8793          	addi	a5,s9,-752 # 1d10 <exit-0xe410>
   1072e:	97ba                	add	a5,a5,a4
   10730:	2fcd0613          	addi	a2,s10,764 # ffffffffffffe2fc <__global_pointer$+0xfffffffffffe5afc>
   10734:	963e                	add	a2,a2,a5
   10736:	8556                	mv	a0,s5
   10738:	85d2                	mv	a1,s4
   1073a:	6a3010ef          	jal	ra,125dc <frexpl>
   1073e:	862a                	mv	a2,a0
   10740:	86ae                	mv	a3,a1
   10742:	5a5020ef          	jal	ra,134e6 <__addtf3>
   10746:	4601                	li	a2,0
   10748:	4681                	li	a3,0
   1074a:	8daa                	mv	s11,a0
   1074c:	8b2e                	mv	s6,a1
   1074e:	8aaa                	mv	s5,a0
   10750:	8a2e                	mv	s4,a1
   10752:	792030ef          	jal	ra,13ee4 <__eqtf2>
   10756:	cd01                	beqz	a0,1076e <fmt_fp+0x170>
   10758:	d10c8793          	addi	a5,s9,-752
   1075c:	0098                	addi	a4,sp,64
   1075e:	97ba                	add	a5,a5,a4
   10760:	97ea                	add	a5,a5,s10
   10762:	2fc7a683          	lw	a3,764(a5)
   10766:	e83e                	sd	a5,16(sp)
   10768:	36fd                	addiw	a3,a3,-1
   1076a:	2ed7ae23          	sw	a3,764(a5)
   1076e:	0209ec93          	ori	s9,s3,32
   10772:	06100693          	li	a3,97
   10776:	2cdc9b63          	bne	s9,a3,10a4c <fmt_fp+0x44e>
   1077a:	0209fd13          	andi	s10,s3,32
   1077e:	000d0363          	beqz	s10,10784 <fmt_fp+0x186>
   10782:	0925                	addi	s2,s2,9
   10784:	002c079b          	addiw	a5,s8,2
   10788:	c83e                	sw	a5,16(sp)
   1078a:	e43e                	sd	a5,8(sp)
   1078c:	0004079b          	sext.w	a5,s0
   10790:	f03e                	sd	a5,32(sp)
   10792:	46e9                	li	a3,26
   10794:	6cd5                	lui	s9,0x15
   10796:	0486e463          	bltu	a3,s0,107de <fmt_fp+0x1e0>
   1079a:	578cba83          	ld	s5,1400(s9) # 15578 <__clzdi2+0x216>
   1079e:	4a6d                	li	s4,27
   107a0:	408a0a3b          	subw	s4,s4,s0
   107a4:	4c01                	li	s8,0
   107a6:	587d                	li	a6,-1
   107a8:	86d6                	mv	a3,s5
   107aa:	3a7d                	addiw	s4,s4,-1
   107ac:	1b0a1063          	bne	s4,a6,1094c <fmt_fp+0x34e>
   107b0:	00094603          	lbu	a2,0(s2)
   107b4:	02d00693          	li	a3,45
   107b8:	1ad61763          	bne	a2,a3,10966 <fmt_fp+0x368>
   107bc:	1a7e                	slli	s4,s4,0x3f
   107be:	86d6                	mv	a3,s5
   107c0:	8662                	mv	a2,s8
   107c2:	014b45b3          	xor	a1,s6,s4
   107c6:	856e                	mv	a0,s11
   107c8:	655030ef          	jal	ra,1461c <__subtf3>
   107cc:	862a                	mv	a2,a0
   107ce:	86ae                	mv	a3,a1
   107d0:	8562                	mv	a0,s8
   107d2:	85d6                	mv	a1,s5
   107d4:	513020ef          	jal	ra,134e6 <__addtf3>
   107d8:	8aaa                	mv	s5,a0
   107da:	0145ca33          	xor	s4,a1,s4
   107de:	6689                	lui	a3,0x2
   107e0:	77f9                	lui	a5,0xffffe
   107e2:	0090                	addi	a2,sp,64
   107e4:	d1068713          	addi	a4,a3,-752 # 1d10 <exit-0xe410>
   107e8:	30078c13          	addi	s8,a5,768 # ffffffffffffe300 <__global_pointer$+0xfffffffffffe5b00>
   107ec:	9732                	add	a4,a4,a2
   107ee:	9c3a                	add	s8,s8,a4
   107f0:	d1068713          	addi	a4,a3,-752
   107f4:	9732                	add	a4,a4,a2
   107f6:	97ba                	add	a5,a5,a4
   107f8:	ec3e                	sd	a5,24(sp)
   107fa:	2fc7a783          	lw	a5,764(a5)
   107fe:	00cc0b13          	addi	s6,s8,12
   10802:	85da                	mv	a1,s6
   10804:	41f7d51b          	sraiw	a0,a5,0x1f
   10808:	8fa9                	xor	a5,a5,a0
   1080a:	40a7853b          	subw	a0,a5,a0
   1080e:	cfbff0ef          	jal	ra,10508 <fmt_u>
   10812:	01651963          	bne	a0,s6,10824 <fmt_fp+0x226>
   10816:	6762                	ld	a4,24(sp)
   10818:	03000793          	li	a5,48
   1081c:	00bc0513          	addi	a0,s8,11
   10820:	30f705a3          	sb	a5,779(a4) # 230b <exit-0xde15>
   10824:	6709                	lui	a4,0x2
   10826:	d1070713          	addi	a4,a4,-752 # 1d10 <exit-0xe410>
   1082a:	0094                	addi	a3,sp,64
   1082c:	9736                	add	a4,a4,a3
   1082e:	77f9                	lui	a5,0xffffe
   10830:	97ba                	add	a5,a5,a4
   10832:	2fc7a703          	lw	a4,764(a5) # ffffffffffffe2fc <__global_pointer$+0xfffffffffffe5afc>
   10836:	ec3e                	sd	a5,24(sp)
   10838:	02d00793          	li	a5,45
   1083c:	00074463          	bltz	a4,10844 <fmt_fp+0x246>
   10840:	02b00793          	li	a5,43
   10844:	6709                	lui	a4,0x2
   10846:	fef50fa3          	sb	a5,-1(a0)
   1084a:	d1070713          	addi	a4,a4,-752 # 1d10 <exit-0xe410>
   1084e:	77f9                	lui	a5,0xffffe
   10850:	0094                	addi	a3,sp,64
   10852:	31078793          	addi	a5,a5,784 # ffffffffffffe310 <__global_pointer$+0xfffffffffffe5b10>
   10856:	9736                	add	a4,a4,a3
   10858:	00f70b33          	add	s6,a4,a5
   1085c:	6755                	lui	a4,0x15
   1085e:	7c070793          	addi	a5,a4,1984 # 157c0 <xdigits>
   10862:	ec3e                	sd	a5,24(sp)
   10864:	578cb783          	ld	a5,1400(s9)
   10868:	29bd                	addiw	s3,s3,15
   1086a:	008bfd93          	andi	s11,s7,8
   1086e:	ffe50c13          	addi	s8,a0,-2
   10872:	ff350f23          	sb	s3,-2(a0)
   10876:	f83e                	sd	a5,48(sp)
   10878:	f45a                	sd	s6,40(sp)
   1087a:	2d81                	sext.w	s11,s11
   1087c:	85d2                	mv	a1,s4
   1087e:	8556                	mv	a0,s5
   10880:	017040ef          	jal	ra,15096 <__fixtfsi>
   10884:	67e2                	ld	a5,24(sp)
   10886:	2501                	sext.w	a0,a0
   10888:	001b0993          	addi	s3,s6,1
   1088c:	00a78733          	add	a4,a5,a0
   10890:	00074703          	lbu	a4,0(a4)
   10894:	00ed6733          	or	a4,s10,a4
   10898:	00eb0023          	sb	a4,0(s6)
   1089c:	11d040ef          	jal	ra,151b8 <__floatsitf>
   108a0:	862a                	mv	a2,a0
   108a2:	86ae                	mv	a3,a1
   108a4:	8556                	mv	a0,s5
   108a6:	85d2                	mv	a1,s4
   108a8:	575030ef          	jal	ra,1461c <__subtf3>
   108ac:	76c2                	ld	a3,48(sp)
   108ae:	4601                	li	a2,0
   108b0:	6c6030ef          	jal	ra,13f76 <__multf3>
   108b4:	77a2                	ld	a5,40(sp)
   108b6:	882a                	mv	a6,a0
   108b8:	8cae                	mv	s9,a1
   108ba:	40f986b3          	sub	a3,s3,a5
   108be:	4785                	li	a5,1
   108c0:	8aaa                	mv	s5,a0
   108c2:	8a2e                	mv	s4,a1
   108c4:	02f69363          	bne	a3,a5,108ea <fmt_fp+0x2ec>
   108c8:	4601                	li	a2,0
   108ca:	4681                	li	a3,0
   108cc:	fc2a                	sd	a0,56(sp)
   108ce:	616030ef          	jal	ra,13ee4 <__eqtf2>
   108d2:	7862                	ld	a6,56(sp)
   108d4:	e509                	bnez	a0,108de <fmt_fp+0x2e0>
   108d6:	00804463          	bgtz	s0,108de <fmt_fp+0x2e0>
   108da:	000d8f63          	beqz	s11,108f8 <fmt_fp+0x2fa>
   108de:	02e00793          	li	a5,46
   108e2:	002b0993          	addi	s3,s6,2
   108e6:	00fb00a3          	sb	a5,1(s6)
   108ea:	4601                	li	a2,0
   108ec:	4681                	li	a3,0
   108ee:	8542                	mv	a0,a6
   108f0:	85e6                	mv	a1,s9
   108f2:	5f2030ef          	jal	ra,13ee4 <__eqtf2>
   108f6:	e549                	bnez	a0,10980 <fmt_fp+0x382>
   108f8:	6789                	lui	a5,0x2
   108fa:	0094                	addi	a3,sp,64
   108fc:	d1078793          	addi	a5,a5,-752 # 1d10 <exit-0xe410>
   10900:	97b6                	add	a5,a5,a3
   10902:	7779                	lui	a4,0xffffe
   10904:	00e78ab3          	add	s5,a5,a4
   10908:	66a2                	ld	a3,8(sp)
   1090a:	30ca8a93          	addi	s5,s5,780
   1090e:	800007b7          	lui	a5,0x80000
   10912:	418a8ab3          	sub	s5,s5,s8
   10916:	ffd7c793          	xori	a5,a5,-3
   1091a:	415787b3          	sub	a5,a5,s5
   1091e:	8f95                	sub	a5,a5,a3
   10920:	0687d263          	bge	a5,s0,10984 <fmt_fp+0x386>
   10924:	557d                	li	a0,-1
   10926:	6289                	lui	t0,0x2
   10928:	d4028293          	addi	t0,t0,-704 # 1d40 <exit-0xe3e0>
   1092c:	9116                	add	sp,sp,t0
   1092e:	70e6                	ld	ra,120(sp)
   10930:	7446                	ld	s0,112(sp)
   10932:	74a6                	ld	s1,104(sp)
   10934:	7906                	ld	s2,96(sp)
   10936:	69e6                	ld	s3,88(sp)
   10938:	6a46                	ld	s4,80(sp)
   1093a:	6aa6                	ld	s5,72(sp)
   1093c:	6b06                	ld	s6,64(sp)
   1093e:	7be2                	ld	s7,56(sp)
   10940:	7c42                	ld	s8,48(sp)
   10942:	7ca2                	ld	s9,40(sp)
   10944:	7d02                	ld	s10,32(sp)
   10946:	6de2                	ld	s11,24(sp)
   10948:	6109                	addi	sp,sp,128
   1094a:	8082                	ret
   1094c:	8562                	mv	a0,s8
   1094e:	85d6                	mv	a1,s5
   10950:	4601                	li	a2,0
   10952:	624030ef          	jal	ra,13f76 <__multf3>
   10956:	040037b7          	lui	a5,0x4003
   1095a:	8c2a                	mv	s8,a0
   1095c:	8aae                	mv	s5,a1
   1095e:	587d                	li	a6,-1
   10960:	02479693          	slli	a3,a5,0x24
   10964:	b599                	j	107aa <fmt_fp+0x1ac>
   10966:	866e                	mv	a2,s11
   10968:	86da                	mv	a3,s6
   1096a:	85d6                	mv	a1,s5
   1096c:	8562                	mv	a0,s8
   1096e:	379020ef          	jal	ra,134e6 <__addtf3>
   10972:	86d6                	mv	a3,s5
   10974:	8662                	mv	a2,s8
   10976:	4a7030ef          	jal	ra,1461c <__subtf3>
   1097a:	8aaa                	mv	s5,a0
   1097c:	8a2e                	mv	s4,a1
   1097e:	b585                	j	107de <fmt_fp+0x1e0>
   10980:	8b4e                	mv	s6,s3
   10982:	bded                	j	1087c <fmt_fp+0x27e>
   10984:	6789                	lui	a5,0x2
   10986:	d1078793          	addi	a5,a5,-752 # 1d10 <exit-0xe410>
   1098a:	0094                	addi	a3,sp,64
   1098c:	97b6                	add	a5,a5,a3
   1098e:	31070713          	addi	a4,a4,784 # ffffffffffffe310 <__global_pointer$+0xfffffffffffe5b10>
   10992:	973e                	add	a4,a4,a5
   10994:	40e989b3          	sub	s3,s3,a4
   10998:	000a879b          	sext.w	a5,s5
   1099c:	013a8dbb          	addw	s11,s5,s3
   109a0:	c05d                	beqz	s0,10a46 <fmt_fp+0x448>
   109a2:	fff98713          	addi	a4,s3,-1
   109a6:	0ae44063          	blt	s0,a4,10a46 <fmt_fp+0x448>
   109aa:	7702                	ld	a4,32(sp)
   109ac:	00270a1b          	addiw	s4,a4,2
   109b0:	00fa0a3b          	addw	s4,s4,a5
   109b4:	47c2                	lw	a5,16(sp)
   109b6:	6602                	ld	a2,0(sp)
   109b8:	875e                	mv	a4,s7
   109ba:	00fa0d3b          	addw	s10,s4,a5
   109be:	86ea                	mv	a3,s10
   109c0:	02000593          	li	a1,32
   109c4:	8526                	mv	a0,s1
   109c6:	bc5ff0ef          	jal	ra,1058a <pad>
   109ca:	6622                	ld	a2,8(sp)
   109cc:	85ca                	mv	a1,s2
   109ce:	8526                	mv	a0,s1
   109d0:	ba5ff0ef          	jal	ra,10574 <out>
   109d4:	6602                	ld	a2,0(sp)
   109d6:	6741                	lui	a4,0x10
   109d8:	86ea                	mv	a3,s10
   109da:	00ebc733          	xor	a4,s7,a4
   109de:	03000593          	li	a1,48
   109e2:	8526                	mv	a0,s1
   109e4:	ba7ff0ef          	jal	ra,1058a <pad>
   109e8:	6789                	lui	a5,0x2
   109ea:	0098                	addi	a4,sp,64
   109ec:	75f9                	lui	a1,0xffffe
   109ee:	d1078793          	addi	a5,a5,-752 # 1d10 <exit-0xe410>
   109f2:	97ba                	add	a5,a5,a4
   109f4:	31058593          	addi	a1,a1,784 # ffffffffffffe310 <__global_pointer$+0xfffffffffffe5b10>
   109f8:	95be                	add	a1,a1,a5
   109fa:	864e                	mv	a2,s3
   109fc:	8526                	mv	a0,s1
   109fe:	b77ff0ef          	jal	ra,10574 <out>
   10a02:	4701                	li	a4,0
   10a04:	4681                	li	a3,0
   10a06:	41ba063b          	subw	a2,s4,s11
   10a0a:	03000593          	li	a1,48
   10a0e:	8526                	mv	a0,s1
   10a10:	b7bff0ef          	jal	ra,1058a <pad>
   10a14:	8656                	mv	a2,s5
   10a16:	85e2                	mv	a1,s8
   10a18:	8526                	mv	a0,s1
   10a1a:	b5bff0ef          	jal	ra,10574 <out>
   10a1e:	6602                	ld	a2,0(sp)
   10a20:	000b891b          	sext.w	s2,s7
   10a24:	6709                	lui	a4,0x2
   10a26:	00e94733          	xor	a4,s2,a4
   10a2a:	86ea                	mv	a3,s10
   10a2c:	02000593          	li	a1,32
   10a30:	8526                	mv	a0,s1
   10a32:	b59ff0ef          	jal	ra,1058a <pad>
   10a36:	6782                	ld	a5,0(sp)
   10a38:	846a                	mv	s0,s10
   10a3a:	00fd5363          	bge	s10,a5,10a40 <fmt_fp+0x442>
   10a3e:	4402                	lw	s0,0(sp)
   10a40:	0004051b          	sext.w	a0,s0
   10a44:	b5cd                	j	10926 <fmt_fp+0x328>
   10a46:	000d8a1b          	sext.w	s4,s11
   10a4a:	b7ad                	j	109b4 <fmt_fp+0x3b6>
   10a4c:	00045363          	bgez	s0,10a52 <fmt_fp+0x454>
   10a50:	4419                	li	s0,6
   10a52:	4601                	li	a2,0
   10a54:	4681                	li	a3,0
   10a56:	856e                	mv	a0,s11
   10a58:	85da                	mv	a1,s6
   10a5a:	48a030ef          	jal	ra,13ee4 <__eqtf2>
   10a5e:	c90d                	beqz	a0,10a90 <fmt_fp+0x492>
   10a60:	66d5                	lui	a3,0x15
   10a62:	5d86b683          	ld	a3,1496(a3) # 155d8 <__clzdi2+0x276>
   10a66:	4601                	li	a2,0
   10a68:	856e                	mv	a0,s11
   10a6a:	85da                	mv	a1,s6
   10a6c:	50a030ef          	jal	ra,13f76 <__multf3>
   10a70:	6709                	lui	a4,0x2
   10a72:	d1070713          	addi	a4,a4,-752 # 1d10 <exit-0xe410>
   10a76:	0094                	addi	a3,sp,64
   10a78:	9736                	add	a4,a4,a3
   10a7a:	77f9                	lui	a5,0xffffe
   10a7c:	97ba                	add	a5,a5,a4
   10a7e:	e83e                	sd	a5,16(sp)
   10a80:	2fc7a783          	lw	a5,764(a5) # ffffffffffffe2fc <__global_pointer$+0xfffffffffffe5afc>
   10a84:	6742                	ld	a4,16(sp)
   10a86:	8aaa                	mv	s5,a0
   10a88:	3791                	addiw	a5,a5,-28
   10a8a:	8a2e                	mv	s4,a1
   10a8c:	2ef72e23          	sw	a5,764(a4)
   10a90:	4336                	lw	t1,76(sp)
   10a92:	6709                	lui	a4,0x2
   10a94:	b4870693          	addi	a3,a4,-1208 # 1b48 <exit-0xe5d8>
   10a98:	0090                	addi	a2,sp,64
   10a9a:	77f9                	lui	a5,0xffffe
   10a9c:	00d608b3          	add	a7,a2,a3
   10aa0:	00035863          	bgez	t1,10ab0 <fmt_fp+0x4b2>
   10aa4:	33878893          	addi	a7,a5,824 # ffffffffffffe338 <__global_pointer$+0xfffffffffffe5b38>
   10aa8:	d1070793          	addi	a5,a4,-752
   10aac:	97b2                	add	a5,a5,a2
   10aae:	98be                	add	a7,a7,a5
   10ab0:	6755                	lui	a4,0x15
   10ab2:	5e873d83          	ld	s11,1512(a4) # 155e8 <__clzdi2+0x286>
   10ab6:	8b46                	mv	s6,a7
   10ab8:	85d2                	mv	a1,s4
   10aba:	8556                	mv	a0,s5
   10abc:	ec46                	sd	a7,24(sp)
   10abe:	e81a                	sd	t1,16(sp)
   10ac0:	67a040ef          	jal	ra,1513a <__fixunstfsi>
   10ac4:	00ab2023          	sw	a0,0(s6)
   10ac8:	2501                	sext.w	a0,a0
   10aca:	752040ef          	jal	ra,1521c <__floatunsitf>
   10ace:	862a                	mv	a2,a0
   10ad0:	86ae                	mv	a3,a1
   10ad2:	8556                	mv	a0,s5
   10ad4:	85d2                	mv	a1,s4
   10ad6:	347030ef          	jal	ra,1461c <__subtf3>
   10ada:	4601                	li	a2,0
   10adc:	86ee                	mv	a3,s11
   10ade:	498030ef          	jal	ra,13f76 <__multf3>
   10ae2:	4601                	li	a2,0
   10ae4:	4681                	li	a3,0
   10ae6:	8aaa                	mv	s5,a0
   10ae8:	8a2e                	mv	s4,a1
   10aea:	3fa030ef          	jal	ra,13ee4 <__eqtf2>
   10aee:	6342                	ld	t1,16(sp)
   10af0:	68e2                	ld	a7,24(sp)
   10af2:	0b11                	addi	s6,s6,4
   10af4:	f171                	bnez	a0,10ab8 <fmt_fp+0x4ba>
   10af6:	3b9ad537          	lui	a0,0x3b9ad
   10afa:	8d46                	mv	s10,a7
   10afc:	4701                	li	a4,0
   10afe:	4e75                	li	t3,29
   10b00:	a0050513          	addi	a0,a0,-1536 # 3b9aca00 <__global_pointer$+0x3b994200>
   10b04:	14604163          	bgtz	t1,10c46 <fmt_fp+0x648>
   10b08:	cb19                	beqz	a4,10b1e <fmt_fp+0x520>
   10b0a:	6789                	lui	a5,0x2
   10b0c:	d1078793          	addi	a5,a5,-752 # 1d10 <exit-0xe410>
   10b10:	0094                	addi	a3,sp,64
   10b12:	7779                	lui	a4,0xffffe
   10b14:	97b6                	add	a5,a5,a3
   10b16:	97ba                	add	a5,a5,a4
   10b18:	e83e                	sd	a5,16(sp)
   10b1a:	2e67ae23          	sw	t1,764(a5)
   10b1e:	46a5                	li	a3,9
   10b20:	02d4071b          	addiw	a4,s0,45
   10b24:	02d7573b          	divuw	a4,a4,a3
   10b28:	6789                	lui	a5,0x2
   10b2a:	0090                	addi	a2,sp,64
   10b2c:	d1078793          	addi	a5,a5,-752 # 1d10 <exit-0xe410>
   10b30:	76f9                	lui	a3,0xffffe
   10b32:	97b2                	add	a5,a5,a2
   10b34:	97b6                	add	a5,a5,a3
   10b36:	2fc7a683          	lw	a3,764(a5)
   10b3a:	3b9ade37          	lui	t3,0x3b9ad
   10b3e:	e83e                	sd	a5,16(sp)
   10b40:	4601                	li	a2,0
   10b42:	5f5d                	li	t5,-9
   10b44:	4f85                	li	t6,1
   10b46:	a00e0e1b          	addiw	t3,t3,-1536
   10b4a:	06600293          	li	t0,102
   10b4e:	2705                	addiw	a4,a4,1
   10b50:	1702                	slli	a4,a4,0x20
   10b52:	9301                	srli	a4,a4,0x20
   10b54:	00271e93          	slli	t4,a4,0x2
   10b58:	1206cf63          	bltz	a3,10c96 <fmt_fp+0x698>
   10b5c:	ca19                	beqz	a2,10b72 <fmt_fp+0x574>
   10b5e:	6789                	lui	a5,0x2
   10b60:	d1078793          	addi	a5,a5,-752 # 1d10 <exit-0xe410>
   10b64:	0090                	addi	a2,sp,64
   10b66:	7779                	lui	a4,0xffffe
   10b68:	97b2                	add	a5,a5,a2
   10b6a:	97ba                	add	a5,a5,a4
   10b6c:	e83e                	sd	a5,16(sp)
   10b6e:	2ed7ae23          	sw	a3,764(a5)
   10b72:	4d81                	li	s11,0
   10b74:	016d7f63          	bgeu	s10,s6,10b92 <fmt_fp+0x594>
   10b78:	41a88db3          	sub	s11,a7,s10
   10b7c:	000d2683          	lw	a3,0(s10)
   10b80:	402dd713          	srai	a4,s11,0x2
   10b84:	4da5                	li	s11,9
   10b86:	02ed8dbb          	mulw	s11,s11,a4
   10b8a:	4629                	li	a2,10
   10b8c:	4729                	li	a4,10
   10b8e:	16e6f663          	bgeu	a3,a4,10cfa <fmt_fp+0x6fc>
   10b92:	f9ac8713          	addi	a4,s9,-102
   10b96:	00e03733          	snez	a4,a4
   10b9a:	06700613          	li	a2,103
   10b9e:	4681                	li	a3,0
   10ba0:	03b7073b          	mulw	a4,a4,s11
   10ba4:	40e4073b          	subw	a4,s0,a4
   10ba8:	00cc9463          	bne	s9,a2,10bb0 <fmt_fp+0x5b2>
   10bac:	008036b3          	snez	a3,s0
   10bb0:	40d7063b          	subw	a2,a4,a3
   10bb4:	411b0733          	sub	a4,s6,a7
   10bb8:	8709                	srai	a4,a4,0x2
   10bba:	177d                	addi	a4,a4,-1
   10bbc:	45a5                	li	a1,9
   10bbe:	02b70733          	mul	a4,a4,a1
   10bc2:	1ae65363          	bge	a2,a4,10d68 <fmt_fp+0x76a>
   10bc6:	00024737          	lui	a4,0x24
   10bca:	9f31                	addw	a4,a4,a2
   10bcc:	4625                	li	a2,9
   10bce:	02c74a3b          	divw	s4,a4,a2
   10bd2:	76c1                	lui	a3,0xffff0
   10bd4:	0691                	addi	a3,a3,4
   10bd6:	45a9                	li	a1,10
   10bd8:	02c7673b          	remw	a4,a4,a2
   10bdc:	0a0a                	slli	s4,s4,0x2
   10bde:	9a36                	add	s4,s4,a3
   10be0:	9a46                	add	s4,s4,a7
   10be2:	46a9                	li	a3,10
   10be4:	4625                	li	a2,9
   10be6:	2705                	addiw	a4,a4,1
   10be8:	10c71d63          	bne	a4,a2,10d02 <fmt_fp+0x704>
   10bec:	000a2703          	lw	a4,0(s4)
   10bf0:	00068e1b          	sext.w	t3,a3
   10bf4:	02d7763b          	remuw	a2,a4,a3
   10bf8:	8532                	mv	a0,a2
   10bfa:	e609                	bnez	a2,10c04 <fmt_fp+0x606>
   10bfc:	004a0593          	addi	a1,s4,4
   10c00:	16bb0063          	beq	s6,a1,10d60 <fmt_fp+0x762>
   10c04:	03c755bb          	divuw	a1,a4,t3
   10c08:	8985                	andi	a1,a1,1
   10c0a:	ed89                	bnez	a1,10c24 <fmt_fp+0x626>
   10c0c:	3b9ad5b7          	lui	a1,0x3b9ad
   10c10:	a0058593          	addi	a1,a1,-1536 # 3b9aca00 <__global_pointer$+0x3b994200>
   10c14:	0eb69b63          	bne	a3,a1,10d0a <fmt_fp+0x70c>
   10c18:	0f4d7963          	bgeu	s10,s4,10d0a <fmt_fp+0x70c>
   10c1c:	ffca2583          	lw	a1,-4(s4)
   10c20:	8985                	andi	a1,a1,1
   10c22:	c5e5                	beqz	a1,10d0a <fmt_fp+0x70c>
   10c24:	65d5                	lui	a1,0x15
   10c26:	5985ba83          	ld	s5,1432(a1) # 15598 <__clzdi2+0x236>
   10c2a:	4305                	li	t1,1
   10c2c:	8685                	srai	a3,a3,0x1
   10c2e:	0ed66363          	bltu	a2,a3,10d14 <fmt_fp+0x716>
   10c32:	00c69663          	bne	a3,a2,10c3e <fmt_fp+0x640>
   10c36:	004a0693          	addi	a3,s4,4
   10c3a:	2adb0a63          	beq	s6,a3,10eee <fmt_fp+0x8f0>
   10c3e:	66d5                	lui	a3,0x15
   10c40:	5a86b683          	ld	a3,1448(a3) # 155a8 <__clzdi2+0x246>
   10c44:	a8d9                	j	10d1a <fmt_fp+0x71c>
   10c46:	0003061b          	sext.w	a2,t1
   10c4a:	006e5363          	bge	t3,t1,10c50 <fmt_fp+0x652>
   10c4e:	4675                	li	a2,29
   10c50:	ffcb0593          	addi	a1,s6,-4
   10c54:	4701                	li	a4,0
   10c56:	01a5ff63          	bgeu	a1,s10,10c74 <fmt_fp+0x676>
   10c5a:	c701                	beqz	a4,10c62 <fmt_fp+0x664>
   10c5c:	feed2e23          	sw	a4,-4(s10)
   10c60:	1d71                	addi	s10,s10,-4
   10c62:	016d7563          	bgeu	s10,s6,10c6c <fmt_fp+0x66e>
   10c66:	ffcb2703          	lw	a4,-4(s6)
   10c6a:	c705                	beqz	a4,10c92 <fmt_fp+0x694>
   10c6c:	40c3033b          	subw	t1,t1,a2
   10c70:	4705                	li	a4,1
   10c72:	bd49                	j	10b04 <fmt_fp+0x506>
   10c74:	0005e683          	lwu	a3,0(a1)
   10c78:	1702                	slli	a4,a4,0x20
   10c7a:	9301                	srli	a4,a4,0x20
   10c7c:	00c696b3          	sll	a3,a3,a2
   10c80:	9736                	add	a4,a4,a3
   10c82:	02a776b3          	remu	a3,a4,a0
   10c86:	15f1                	addi	a1,a1,-4
   10c88:	02a75733          	divu	a4,a4,a0
   10c8c:	c1d4                	sw	a3,4(a1)
   10c8e:	2701                	sext.w	a4,a4
   10c90:	b7d9                	j	10c56 <fmt_fp+0x658>
   10c92:	1b71                	addi	s6,s6,-4
   10c94:	b7f9                	j	10c62 <fmt_fp+0x664>
   10c96:	45a5                	li	a1,9
   10c98:	01e6c463          	blt	a3,t5,10ca0 <fmt_fp+0x6a2>
   10c9c:	40d005bb          	negw	a1,a3
   10ca0:	00bf93bb          	sllw	t2,t6,a1
   10ca4:	33fd                	addiw	t2,t2,-1
   10ca6:	40be5a3b          	sraw	s4,t3,a1
   10caa:	866a                	mv	a2,s10
   10cac:	4501                	li	a0,0
   10cae:	03666863          	bltu	a2,s6,10cde <fmt_fp+0x6e0>
   10cb2:	000d2603          	lw	a2,0(s10)
   10cb6:	e211                	bnez	a2,10cba <fmt_fp+0x6bc>
   10cb8:	0d11                	addi	s10,s10,4
   10cba:	c501                	beqz	a0,10cc2 <fmt_fp+0x6c4>
   10cbc:	00ab2023          	sw	a0,0(s6)
   10cc0:	0b11                	addi	s6,s6,4
   10cc2:	8646                	mv	a2,a7
   10cc4:	005c8363          	beq	s9,t0,10cca <fmt_fp+0x6cc>
   10cc8:	866a                	mv	a2,s10
   10cca:	40cb0533          	sub	a0,s6,a2
   10cce:	8509                	srai	a0,a0,0x2
   10cd0:	00a75463          	bge	a4,a0,10cd8 <fmt_fp+0x6da>
   10cd4:	01d60b33          	add	s6,a2,t4
   10cd8:	9ead                	addw	a3,a3,a1
   10cda:	4605                	li	a2,1
   10cdc:	bdb5                	j	10b58 <fmt_fp+0x55a>
   10cde:	00062303          	lw	t1,0(a2) # 8000 <exit-0x8120>
   10ce2:	0611                	addi	a2,a2,4
   10ce4:	00b35abb          	srlw	s5,t1,a1
   10ce8:	00aa853b          	addw	a0,s5,a0
   10cec:	fea62e23          	sw	a0,-4(a2)
   10cf0:	00737533          	and	a0,t1,t2
   10cf4:	0345053b          	mulw	a0,a0,s4
   10cf8:	bf5d                	j	10cae <fmt_fp+0x6b0>
   10cfa:	02e6073b          	mulw	a4,a2,a4
   10cfe:	2d85                	addiw	s11,s11,1
   10d00:	b579                	j	10b8e <fmt_fp+0x590>
   10d02:	02d586bb          	mulw	a3,a1,a3
   10d06:	2705                	addiw	a4,a4,1
   10d08:	b5c5                	j	10be8 <fmt_fp+0x5ea>
   10d0a:	65d5                	lui	a1,0x15
   10d0c:	5885ba83          	ld	s5,1416(a1) # 15588 <__clzdi2+0x226>
   10d10:	4301                	li	t1,0
   10d12:	bf29                	j	10c2c <fmt_fp+0x62e>
   10d14:	66d5                	lui	a3,0x15
   10d16:	5c86b683          	ld	a3,1480(a3) # 155c8 <__clzdi2+0x266>
   10d1a:	000c0d63          	beqz	s8,10d34 <fmt_fp+0x736>
   10d1e:	00094583          	lbu	a1,0(s2)
   10d22:	02d00613          	li	a2,45
   10d26:	00c59763          	bne	a1,a2,10d34 <fmt_fp+0x736>
   10d2a:	567d                	li	a2,-1
   10d2c:	167e                	slli	a2,a2,0x3f
   10d2e:	00cacab3          	xor	s5,s5,a2
   10d32:	8eb1                	xor	a3,a3,a2
   10d34:	9f09                	subw	a4,a4,a0
   10d36:	4601                	li	a2,0
   10d38:	851a                	mv	a0,t1
   10d3a:	85d6                	mv	a1,s5
   10d3c:	f446                	sd	a7,40(sp)
   10d3e:	f072                	sd	t3,32(sp)
   10d40:	ec3a                	sd	a4,24(sp)
   10d42:	e81a                	sd	t1,16(sp)
   10d44:	7a2020ef          	jal	ra,134e6 <__addtf3>
   10d48:	6342                	ld	t1,16(sp)
   10d4a:	86d6                	mv	a3,s5
   10d4c:	861a                	mv	a2,t1
   10d4e:	196030ef          	jal	ra,13ee4 <__eqtf2>
   10d52:	6762                	ld	a4,24(sp)
   10d54:	7e02                	ld	t3,32(sp)
   10d56:	78a2                	ld	a7,40(sp)
   10d58:	18051f63          	bnez	a0,10ef6 <fmt_fp+0x8f8>
   10d5c:	00ea2023          	sw	a4,0(s4)
   10d60:	0a11                	addi	s4,s4,4
   10d62:	016a7363          	bgeu	s4,s6,10d68 <fmt_fp+0x76a>
   10d66:	8b52                	mv	s6,s4
   10d68:	016d7663          	bgeu	s10,s6,10d74 <fmt_fp+0x776>
   10d6c:	ffcb2703          	lw	a4,-4(s6)
   10d70:	1c070e63          	beqz	a4,10f4c <fmt_fp+0x94e>
   10d74:	06700713          	li	a4,103
   10d78:	04ec9d63          	bne	s9,a4,10dd2 <fmt_fp+0x7d4>
   10d7c:	e011                	bnez	s0,10d80 <fmt_fp+0x782>
   10d7e:	4405                	li	s0,1
   10d80:	1c8dd863          	bge	s11,s0,10f50 <fmt_fp+0x952>
   10d84:	5771                	li	a4,-4
   10d86:	1cedc563          	blt	s11,a4,10f50 <fmt_fp+0x952>
   10d8a:	001d871b          	addiw	a4,s11,1
   10d8e:	39fd                	addiw	s3,s3,-1
   10d90:	9c19                	subw	s0,s0,a4
   10d92:	008bf713          	andi	a4,s7,8
   10d96:	ef15                	bnez	a4,10dd2 <fmt_fp+0x7d4>
   10d98:	46a5                	li	a3,9
   10d9a:	016d7663          	bgeu	s10,s6,10da6 <fmt_fp+0x7a8>
   10d9e:	ffcb2603          	lw	a2,-4(s6)
   10da2:	1c061163          	bnez	a2,10f64 <fmt_fp+0x966>
   10da6:	411b0733          	sub	a4,s6,a7
   10daa:	8709                	srai	a4,a4,0x2
   10dac:	4625                	li	a2,9
   10dae:	177d                	addi	a4,a4,-1
   10db0:	02c70733          	mul	a4,a4,a2
   10db4:	06600593          	li	a1,102
   10db8:	0209e613          	ori	a2,s3,32
   10dbc:	1ab61863          	bne	a2,a1,10f6c <fmt_fp+0x96e>
   10dc0:	8f15                	sub	a4,a4,a3
   10dc2:	00075363          	bgez	a4,10dc8 <fmt_fp+0x7ca>
   10dc6:	4701                	li	a4,0
   10dc8:	00e45363          	bge	s0,a4,10dce <fmt_fp+0x7d0>
   10dcc:	8722                	mv	a4,s0
   10dce:	0007041b          	sext.w	s0,a4
   10dd2:	80000737          	lui	a4,0x80000
   10dd6:	ffd74713          	xori	a4,a4,-3
   10dda:	b48745e3          	blt	a4,s0,10924 <fmt_fp+0x326>
   10dde:	000b879b          	sext.w	a5,s7
   10de2:	008bfa13          	andi	s4,s7,8
   10de6:	e83e                	sd	a5,16(sp)
   10de8:	000a079b          	sext.w	a5,s4
   10dec:	01446a33          	or	s4,s0,s4
   10df0:	014036b3          	snez	a3,s4
   10df4:	0014071b          	addiw	a4,s0,1
   10df8:	9f35                	addw	a4,a4,a3
   10dfa:	800006b7          	lui	a3,0x80000
   10dfe:	fff6c693          	not	a3,a3
   10e02:	ec3e                	sd	a5,24(sp)
   10e04:	0209ea93          	ori	s5,s3,32
   10e08:	06600613          	li	a2,102
   10e0c:	0007051b          	sext.w	a0,a4
   10e10:	9e99                	subw	a3,a3,a4
   10e12:	14ca9f63          	bne	s5,a2,10f70 <fmt_fp+0x972>
   10e16:	b1b6c7e3          	blt	a3,s11,10924 <fmt_fp+0x326>
   10e1a:	01b05463          	blez	s11,10e22 <fmt_fp+0x824>
   10e1e:	01b7053b          	addw	a0,a4,s11
   10e22:	80000737          	lui	a4,0x80000
   10e26:	1c0c1563          	bnez	s8,10ff0 <fmt_fp+0x9f2>
   10e2a:	fff74713          	not	a4,a4
   10e2e:	f046                	sd	a7,32(sp)
   10e30:	2701                	sext.w	a4,a4
   10e32:	aea749e3          	blt	a4,a0,10924 <fmt_fp+0x326>
   10e36:	6602                	ld	a2,0(sp)
   10e38:	01850dbb          	addw	s11,a0,s8
   10e3c:	875e                	mv	a4,s7
   10e3e:	86ee                	mv	a3,s11
   10e40:	02000593          	li	a1,32
   10e44:	8526                	mv	a0,s1
   10e46:	f44ff0ef          	jal	ra,1058a <pad>
   10e4a:	8662                	mv	a2,s8
   10e4c:	85ca                	mv	a1,s2
   10e4e:	8526                	mv	a0,s1
   10e50:	f24ff0ef          	jal	ra,10574 <out>
   10e54:	67c2                	ld	a5,16(sp)
   10e56:	6602                	ld	a2,0(sp)
   10e58:	6741                	lui	a4,0x10
   10e5a:	8f3d                	xor	a4,a4,a5
   10e5c:	86ee                	mv	a3,s11
   10e5e:	03000593          	li	a1,48
   10e62:	8526                	mv	a0,s1
   10e64:	f26ff0ef          	jal	ra,1058a <pad>
   10e68:	06600713          	li	a4,102
   10e6c:	7882                	ld	a7,32(sp)
   10e6e:	89ee                	mv	s3,s11
   10e70:	24ea9563          	bne	s5,a4,110ba <fmt_fp+0xabc>
   10e74:	8946                	mv	s2,a7
   10e76:	011d7363          	bgeu	s10,a7,10e7c <fmt_fp+0x87e>
   10e7a:	896a                	mv	s2,s10
   10e7c:	6709                	lui	a4,0x2
   10e7e:	7bf9                	lui	s7,0xffffe
   10e80:	0094                	addi	a3,sp,64
   10e82:	d1070793          	addi	a5,a4,-752 # 1d10 <exit-0xe410>
   10e86:	310b8c93          	addi	s9,s7,784 # ffffffffffffe310 <__global_pointer$+0xfffffffffffe5b10>
   10e8a:	97b6                	add	a5,a5,a3
   10e8c:	9cbe                	add	s9,s9,a5
   10e8e:	d1070793          	addi	a5,a4,-752
   10e92:	97b6                	add	a5,a5,a3
   10e94:	8d4a                	mv	s10,s2
   10e96:	009c8a93          	addi	s5,s9,9
   10e9a:	03000c13          	li	s8,48
   10e9e:	9bbe                	add	s7,s7,a5
   10ea0:	15a8fb63          	bgeu	a7,s10,10ff6 <fmt_fp+0x9f8>
   10ea4:	00188693          	addi	a3,a7,1
   10ea8:	ffd90713          	addi	a4,s2,-3
   10eac:	4601                	li	a2,0
   10eae:	00e6e763          	bltu	a3,a4,10ebc <fmt_fp+0x8be>
   10eb2:	0891                	addi	a7,a7,4
   10eb4:	412888b3          	sub	a7,a7,s2
   10eb8:	ffc8f613          	andi	a2,a7,-4
   10ebc:	9932                	add	s2,s2,a2
   10ebe:	1e0a0c63          	beqz	s4,110b6 <fmt_fp+0xab8>
   10ec2:	65d5                	lui	a1,0x15
   10ec4:	4605                	li	a2,1
   10ec6:	45058593          	addi	a1,a1,1104 # 15450 <__clzdi2+0xee>
   10eca:	8526                	mv	a0,s1
   10ecc:	ea8ff0ef          	jal	ra,10574 <out>
   10ed0:	6789                	lui	a5,0x2
   10ed2:	7a79                	lui	s4,0xffffe
   10ed4:	d1078793          	addi	a5,a5,-752 # 1d10 <exit-0xe410>
   10ed8:	0098                	addi	a4,sp,64
   10eda:	310a0a13          	addi	s4,s4,784 # ffffffffffffe310 <__global_pointer$+0xfffffffffffe5b10>
   10ede:	97ba                	add	a5,a5,a4
   10ee0:	9a3e                	add	s4,s4,a5
   10ee2:	009a0b93          	addi	s7,s4,9
   10ee6:	03000c13          	li	s8,48
   10eea:	4ca5                	li	s9,9
   10eec:	a259                	j	11072 <fmt_fp+0xa74>
   10eee:	66d5                	lui	a3,0x15
   10ef0:	5b86b683          	ld	a3,1464(a3) # 155b8 <__clzdi2+0x256>
   10ef4:	b51d                	j	10d1a <fmt_fp+0x71c>
   10ef6:	00ee073b          	addw	a4,t3,a4
   10efa:	00ea2023          	sw	a4,0(s4)
   10efe:	3b9ad737          	lui	a4,0x3b9ad
   10f02:	9ff70713          	addi	a4,a4,-1537 # 3b9ac9ff <__global_pointer$+0x3b9941ff>
   10f06:	000a2683          	lw	a3,0(s4)
   10f0a:	02d76363          	bltu	a4,a3,10f30 <fmt_fp+0x932>
   10f0e:	41a88db3          	sub	s11,a7,s10
   10f12:	000d2683          	lw	a3,0(s10)
   10f16:	402dd713          	srai	a4,s11,0x2
   10f1a:	4da5                	li	s11,9
   10f1c:	02ed8dbb          	mulw	s11,s11,a4
   10f20:	4629                	li	a2,10
   10f22:	4729                	li	a4,10
   10f24:	e2e6eee3          	bltu	a3,a4,10d60 <fmt_fp+0x762>
   10f28:	2d85                	addiw	s11,s11,1
   10f2a:	02e6073b          	mulw	a4,a2,a4
   10f2e:	bfdd                	j	10f24 <fmt_fp+0x926>
   10f30:	1a71                	addi	s4,s4,-4
   10f32:	000a2223          	sw	zero,4(s4)
   10f36:	01aa7563          	bgeu	s4,s10,10f40 <fmt_fp+0x942>
   10f3a:	fe0d2e23          	sw	zero,-4(s10)
   10f3e:	1d71                	addi	s10,s10,-4
   10f40:	000a2683          	lw	a3,0(s4)
   10f44:	2685                	addiw	a3,a3,1
   10f46:	00da2023          	sw	a3,0(s4)
   10f4a:	bf75                	j	10f06 <fmt_fp+0x908>
   10f4c:	1b71                	addi	s6,s6,-4
   10f4e:	bd29                	j	10d68 <fmt_fp+0x76a>
   10f50:	39f9                	addiw	s3,s3,-2
   10f52:	347d                	addiw	s0,s0,-1
   10f54:	bd3d                	j	10d92 <fmt_fp+0x794>
   10f56:	02e5073b          	mulw	a4,a0,a4
   10f5a:	2685                	addiw	a3,a3,1
   10f5c:	02e675bb          	remuw	a1,a2,a4
   10f60:	d9fd                	beqz	a1,10f56 <fmt_fp+0x958>
   10f62:	b591                	j	10da6 <fmt_fp+0x7a8>
   10f64:	4681                	li	a3,0
   10f66:	4729                	li	a4,10
   10f68:	4529                	li	a0,10
   10f6a:	bfcd                	j	10f5c <fmt_fp+0x95e>
   10f6c:	976e                	add	a4,a4,s11
   10f6e:	bd89                	j	10dc0 <fmt_fp+0x7c2>
   10f70:	6789                	lui	a5,0x2
   10f72:	f43a                	sd	a4,40(sp)
   10f74:	d1078793          	addi	a5,a5,-752 # 1d10 <exit-0xe410>
   10f78:	0098                	addi	a4,sp,64
   10f7a:	97ba                	add	a5,a5,a4
   10f7c:	7cf9                	lui	s9,0xffffe
   10f7e:	41fdd61b          	sraiw	a2,s11,0x1f
   10f82:	9cbe                	add	s9,s9,a5
   10f84:	00cdc533          	xor	a0,s11,a2
   10f88:	30cc8c93          	addi	s9,s9,780 # ffffffffffffe30c <__global_pointer$+0xfffffffffffe5b0c>
   10f8c:	9d11                	subw	a0,a0,a2
   10f8e:	85e6                	mv	a1,s9
   10f90:	f036                	sd	a3,32(sp)
   10f92:	e446                	sd	a7,8(sp)
   10f94:	d74ff0ef          	jal	ra,10508 <fmt_u>
   10f98:	68a2                	ld	a7,8(sp)
   10f9a:	7682                	ld	a3,32(sp)
   10f9c:	7722                	ld	a4,40(sp)
   10f9e:	4605                	li	a2,1
   10fa0:	03000313          	li	t1,48
   10fa4:	40ac85b3          	sub	a1,s9,a0
   10fa8:	04b65063          	bge	a2,a1,10fe8 <fmt_fp+0x9ea>
   10fac:	02d00613          	li	a2,45
   10fb0:	000dc463          	bltz	s11,10fb8 <fmt_fp+0x9ba>
   10fb4:	02b00613          	li	a2,43
   10fb8:	ffe50793          	addi	a5,a0,-2
   10fbc:	e43e                	sd	a5,8(sp)
   10fbe:	6789                	lui	a5,0x2
   10fc0:	fec50fa3          	sb	a2,-1(a0)
   10fc4:	d1078793          	addi	a5,a5,-752 # 1d10 <exit-0xe410>
   10fc8:	0090                	addi	a2,sp,64
   10fca:	97b2                	add	a5,a5,a2
   10fcc:	ff350f23          	sb	s3,-2(a0)
   10fd0:	79f9                	lui	s3,0xffffe
   10fd2:	99be                	add	s3,s3,a5
   10fd4:	67a2                	ld	a5,8(sp)
   10fd6:	30c98993          	addi	s3,s3,780 # ffffffffffffe30c <__global_pointer$+0xfffffffffffe5b0c>
   10fda:	40f989b3          	sub	s3,s3,a5
   10fde:	9536c3e3          	blt	a3,s3,10924 <fmt_fp+0x326>
   10fe2:	0137053b          	addw	a0,a4,s3
   10fe6:	bd35                	j	10e22 <fmt_fp+0x824>
   10fe8:	157d                	addi	a0,a0,-1
   10fea:	00650023          	sb	t1,0(a0)
   10fee:	bf5d                	j	10fa4 <fmt_fp+0x9a6>
   10ff0:	ffe74713          	xori	a4,a4,-2
   10ff4:	bd2d                	j	10e2e <fmt_fp+0x830>
   10ff6:	000d6503          	lwu	a0,0(s10)
   10ffa:	85d6                	mv	a1,s5
   10ffc:	e446                	sd	a7,8(sp)
   10ffe:	d0aff0ef          	jal	ra,10508 <fmt_u>
   11002:	68a2                	ld	a7,8(sp)
   11004:	85aa                	mv	a1,a0
   11006:	872a                	mv	a4,a0
   11008:	012d1c63          	bne	s10,s2,11020 <fmt_fp+0xa22>
   1100c:	03551263          	bne	a0,s5,11030 <fmt_fp+0xa32>
   11010:	318b8c23          	sb	s8,792(s7)
   11014:	008c8593          	addi	a1,s9,8
   11018:	a821                	j	11030 <fmt_fp+0xa32>
   1101a:	177d                	addi	a4,a4,-1
   1101c:	01870023          	sb	s8,0(a4)
   11020:	feecede3          	bltu	s9,a4,1101a <fmt_fp+0xa1c>
   11024:	4701                	li	a4,0
   11026:	0195e463          	bltu	a1,s9,1102e <fmt_fp+0xa30>
   1102a:	40bc8733          	sub	a4,s9,a1
   1102e:	95ba                	add	a1,a1,a4
   11030:	40ba8633          	sub	a2,s5,a1
   11034:	8526                	mv	a0,s1
   11036:	e446                	sd	a7,8(sp)
   11038:	d3cff0ef          	jal	ra,10574 <out>
   1103c:	68a2                	ld	a7,8(sp)
   1103e:	0d11                	addi	s10,s10,4
   11040:	b585                	j	10ea0 <fmt_fp+0x8a2>
   11042:	00096503          	lwu	a0,0(s2)
   11046:	85de                	mv	a1,s7
   11048:	cc0ff0ef          	jal	ra,10508 <fmt_u>
   1104c:	872a                	mv	a4,a0
   1104e:	06ea6063          	bltu	s4,a4,110ae <fmt_fp+0xab0>
   11052:	4581                	li	a1,0
   11054:	01456463          	bltu	a0,s4,1105c <fmt_fp+0xa5e>
   11058:	40aa05b3          	sub	a1,s4,a0
   1105c:	0004061b          	sext.w	a2,s0
   11060:	008cd363          	bge	s9,s0,11066 <fmt_fp+0xa68>
   11064:	4625                	li	a2,9
   11066:	95aa                	add	a1,a1,a0
   11068:	8526                	mv	a0,s1
   1106a:	d0aff0ef          	jal	ra,10574 <out>
   1106e:	0911                	addi	s2,s2,4
   11070:	345d                	addiw	s0,s0,-9
   11072:	01697463          	bgeu	s2,s6,1107a <fmt_fp+0xa7c>
   11076:	fc8046e3          	bgtz	s0,11042 <fmt_fp+0xa44>
   1107a:	4701                	li	a4,0
   1107c:	46a5                	li	a3,9
   1107e:	0094061b          	addiw	a2,s0,9
   11082:	03000593          	li	a1,48
   11086:	8526                	mv	a0,s1
   11088:	d02ff0ef          	jal	ra,1058a <pad>
   1108c:	67c2                	ld	a5,16(sp)
   1108e:	6602                	ld	a2,0(sp)
   11090:	6709                	lui	a4,0x2
   11092:	8f3d                	xor	a4,a4,a5
   11094:	86ee                	mv	a3,s11
   11096:	02000593          	li	a1,32
   1109a:	8526                	mv	a0,s1
   1109c:	ceeff0ef          	jal	ra,1058a <pad>
   110a0:	6782                	ld	a5,0(sp)
   110a2:	e2fdcf63          	blt	s11,a5,106e0 <fmt_fp+0xe2>
   110a6:	0009851b          	sext.w	a0,s3
   110aa:	87dff06f          	j	10926 <fmt_fp+0x328>
   110ae:	177d                	addi	a4,a4,-1
   110b0:	01870023          	sb	s8,0(a4) # 2000 <exit-0xe120>
   110b4:	bf69                	j	1104e <fmt_fp+0xa50>
   110b6:	4401                	li	s0,0
   110b8:	bd21                	j	10ed0 <fmt_fp+0x8d2>
   110ba:	016d6463          	bltu	s10,s6,110c2 <fmt_fp+0xac4>
   110be:	004d0b13          	addi	s6,s10,4
   110c2:	6689                	lui	a3,0x2
   110c4:	7779                	lui	a4,0xffffe
   110c6:	0090                	addi	a2,sp,64
   110c8:	d1068793          	addi	a5,a3,-752 # 1d10 <exit-0xe410>
   110cc:	97b2                	add	a5,a5,a2
   110ce:	31070913          	addi	s2,a4,784 # ffffffffffffe310 <__global_pointer$+0xfffffffffffe5b10>
   110d2:	993e                	add	s2,s2,a5
   110d4:	d1068793          	addi	a5,a3,-752
   110d8:	97b2                	add	a5,a5,a2
   110da:	97ba                	add	a5,a5,a4
   110dc:	f03e                	sd	a5,32(sp)
   110de:	00890793          	addi	a5,s2,8
   110e2:	8a6a                	mv	s4,s10
   110e4:	00990b93          	addi	s7,s2,9
   110e8:	03000c13          	li	s8,48
   110ec:	f43e                	sd	a5,40(sp)
   110ee:	016a7463          	bgeu	s4,s6,110f6 <fmt_fp+0xaf8>
   110f2:	02045b63          	bgez	s0,11128 <fmt_fp+0xb2a>
   110f6:	0124061b          	addiw	a2,s0,18
   110fa:	8526                	mv	a0,s1
   110fc:	4701                	li	a4,0
   110fe:	46c9                	li	a3,18
   11100:	03000593          	li	a1,48
   11104:	c86ff0ef          	jal	ra,1058a <pad>
   11108:	6789                	lui	a5,0x2
   1110a:	0098                	addi	a4,sp,64
   1110c:	d1078793          	addi	a5,a5,-752 # 1d10 <exit-0xe410>
   11110:	97ba                	add	a5,a5,a4
   11112:	7679                	lui	a2,0xffffe
   11114:	963e                	add	a2,a2,a5
   11116:	67a2                	ld	a5,8(sp)
   11118:	30c60613          	addi	a2,a2,780 # ffffffffffffe30c <__global_pointer$+0xfffffffffffe5b0c>
   1111c:	8526                	mv	a0,s1
   1111e:	8e1d                	sub	a2,a2,a5
   11120:	85be                	mv	a1,a5
   11122:	c52ff0ef          	jal	ra,10574 <out>
   11126:	b79d                	j	1108c <fmt_fp+0xa8e>
   11128:	000a6503          	lwu	a0,0(s4)
   1112c:	85de                	mv	a1,s7
   1112e:	bdaff0ef          	jal	ra,10508 <fmt_u>
   11132:	85aa                	mv	a1,a0
   11134:	01751663          	bne	a0,s7,11140 <fmt_fp+0xb42>
   11138:	7782                	ld	a5,32(sp)
   1113a:	75a2                	ld	a1,40(sp)
   1113c:	31878c23          	sb	s8,792(a5)
   11140:	00040c9b          	sext.w	s9,s0
   11144:	872e                	mv	a4,a1
   11146:	03aa1763          	bne	s4,s10,11174 <fmt_fp+0xb76>
   1114a:	4605                	li	a2,1
   1114c:	8526                	mv	a0,s1
   1114e:	00158a93          	addi	s5,a1,1
   11152:	c22ff0ef          	jal	ra,10574 <out>
   11156:	67e2                	ld	a5,24(sp)
   11158:	0197e733          	or	a4,a5,s9
   1115c:	c70d                	beqz	a4,11186 <fmt_fp+0xb88>
   1115e:	67d5                	lui	a5,0x15
   11160:	4605                	li	a2,1
   11162:	45078593          	addi	a1,a5,1104 # 15450 <__clzdi2+0xee>
   11166:	8526                	mv	a0,s1
   11168:	c0cff0ef          	jal	ra,10574 <out>
   1116c:	a829                	j	11186 <fmt_fp+0xb88>
   1116e:	177d                	addi	a4,a4,-1
   11170:	01870023          	sb	s8,0(a4)
   11174:	fee96de3          	bltu	s2,a4,1116e <fmt_fp+0xb70>
   11178:	4681                	li	a3,0
   1117a:	0125e463          	bltu	a1,s2,11182 <fmt_fp+0xb84>
   1117e:	40b906b3          	sub	a3,s2,a1
   11182:	00d58ab3          	add	s5,a1,a3
   11186:	415b88b3          	sub	a7,s7,s5
   1118a:	8622                	mv	a2,s0
   1118c:	0088d363          	bge	a7,s0,11192 <fmt_fp+0xb94>
   11190:	8646                	mv	a2,a7
   11192:	85d6                	mv	a1,s5
   11194:	8526                	mv	a0,s1
   11196:	f846                	sd	a7,48(sp)
   11198:	bdcff0ef          	jal	ra,10574 <out>
   1119c:	78c2                	ld	a7,48(sp)
   1119e:	0a11                	addi	s4,s4,4
   111a0:	411c843b          	subw	s0,s9,a7
   111a4:	b7a9                	j	110ee <fmt_fp+0xaf0>

00000000000111a6 <printf_core>:
   111a6:	716d                	addi	sp,sp,-272
   111a8:	67d5                	lui	a5,0x15
   111aa:	5f078793          	addi	a5,a5,1520 # 155f0 <states>
   111ae:	f96a                	sd	s10,176(sp)
   111b0:	6d55                	lui	s10,0x15
   111b2:	f5ce                	sd	s3,232(sp)
   111b4:	e83e                	sd	a5,16(sp)
   111b6:	69d5                	lui	s3,0x15
   111b8:	470d0793          	addi	a5,s10,1136 # 15470 <__clzdi2+0x10e>
   111bc:	e222                	sd	s0,256(sp)
   111be:	f9ca                	sd	s2,240(sp)
   111c0:	f1d2                	sd	s4,224(sp)
   111c2:	e9da                	sd	s6,208(sp)
   111c4:	e1e2                	sd	s8,192(sp)
   111c6:	fd66                	sd	s9,184(sp)
   111c8:	e606                	sd	ra,264(sp)
   111ca:	fda6                	sd	s1,248(sp)
   111cc:	edd6                	sd	s5,216(sp)
   111ce:	e5de                	sd	s7,200(sp)
   111d0:	f56e                	sd	s11,168(sp)
   111d2:	842a                	mv	s0,a0
   111d4:	8a32                	mv	s4,a2
   111d6:	e036                	sd	a3,0(sp)
   111d8:	8b3a                	mv	s6,a4
   111da:	e0ae                	sd	a1,64(sp)
   111dc:	4c81                	li	s9,0
   111de:	4901                	li	s2,0
   111e0:	4c01                	li	s8,0
   111e2:	45898993          	addi	s3,s3,1112 # 15458 <__clzdi2+0xf6>
   111e6:	ec3e                	sd	a5,24(sp)
   111e8:	6b86                	ld	s7,64(sp)
   111ea:	0199093b          	addw	s2,s2,s9
   111ee:	8dca                	mv	s11,s2
   111f0:	000bc783          	lbu	a5,0(s7)
   111f4:	5e078363          	beqz	a5,117da <printf_core+0x634>
   111f8:	02500713          	li	a4,37
   111fc:	6306                	ld	t1,64(sp)
   111fe:	00034783          	lbu	a5,0(t1)
   11202:	c399                	beqz	a5,11208 <printf_core+0x62>
   11204:	00e79563          	bne	a5,a4,1120e <printf_core+0x68>
   11208:	02500713          	li	a4,37
   1120c:	a039                	j	1121a <printf_core+0x74>
   1120e:	0305                	addi	t1,t1,1
   11210:	e09a                	sd	t1,64(sp)
   11212:	b7ed                	j	111fc <printf_core+0x56>
   11214:	0789                	addi	a5,a5,2
   11216:	0305                	addi	t1,t1,1
   11218:	e0be                	sd	a5,64(sp)
   1121a:	6786                	ld	a5,64(sp)
   1121c:	0007c683          	lbu	a3,0(a5)
   11220:	00e69663          	bne	a3,a4,1122c <printf_core+0x86>
   11224:	0017c683          	lbu	a3,1(a5)
   11228:	fee686e3          	beq	a3,a4,11214 <printf_core+0x6e>
   1122c:	80000ab7          	lui	s5,0x80000
   11230:	fffaca93          	not	s5,s5
   11234:	41ba87bb          	subw	a5,s5,s11
   11238:	41730333          	sub	t1,t1,s7
   1123c:	e43e                	sd	a5,8(sp)
   1123e:	5467c163          	blt	a5,t1,11780 <printf_core+0x5da>
   11242:	00030c9b          	sext.w	s9,t1
   11246:	c411                	beqz	s0,11252 <printf_core+0xac>
   11248:	8666                	mv	a2,s9
   1124a:	85de                	mv	a1,s7
   1124c:	8522                	mv	a0,s0
   1124e:	b26ff0ef          	jal	ra,10574 <out>
   11252:	520c9463          	bnez	s9,1177a <printf_core+0x5d4>
   11256:	6786                	ld	a5,64(sp)
   11258:	4725                	li	a4,9
   1125a:	0017c683          	lbu	a3,1(a5)
   1125e:	fd06869b          	addiw	a3,a3,-48
   11262:	12d76c63          	bltu	a4,a3,1139a <printf_core+0x1f4>
   11266:	0027c603          	lbu	a2,2(a5)
   1126a:	02400713          	li	a4,36
   1126e:	12e61663          	bne	a2,a4,1139a <printf_core+0x1f4>
   11272:	078d                	addi	a5,a5,3
   11274:	e0be                	sd	a5,64(sp)
   11276:	4c05                	li	s8,1
   11278:	654d                	lui	a0,0x13
   1127a:	4d01                	li	s10,0
   1127c:	48fd                	li	a7,31
   1127e:	8895051b          	addiw	a0,a0,-1911
   11282:	4e85                	li	t4,1
   11284:	6706                	ld	a4,64(sp)
   11286:	00074803          	lbu	a6,0(a4)
   1128a:	fe08061b          	addiw	a2,a6,-32
   1128e:	00c8e763          	bltu	a7,a2,1129c <printf_core+0xf6>
   11292:	00c555bb          	srlw	a1,a0,a2
   11296:	8985                	andi	a1,a1,1
   11298:	10059563          	bnez	a1,113a2 <printf_core+0x1fc>
   1129c:	02a00613          	li	a2,42
   112a0:	12c81e63          	bne	a6,a2,113dc <printf_core+0x236>
   112a4:	00174603          	lbu	a2,1(a4)
   112a8:	45a5                	li	a1,9
   112aa:	fd06051b          	addiw	a0,a2,-48
   112ae:	10a5e363          	bltu	a1,a0,113b4 <printf_core+0x20e>
   112b2:	00274503          	lbu	a0,2(a4)
   112b6:	02400593          	li	a1,36
   112ba:	0eb51d63          	bne	a0,a1,113b4 <printf_core+0x20e>
   112be:	060a                	slli	a2,a2,0x2
   112c0:	965a                	add	a2,a2,s6
   112c2:	45a9                	li	a1,10
   112c4:	f4b62023          	sw	a1,-192(a2)
   112c8:	00174603          	lbu	a2,1(a4)
   112cc:	6782                	ld	a5,0(sp)
   112ce:	070d                	addi	a4,a4,3
   112d0:	0612                	slli	a2,a2,0x4
   112d2:	963e                	add	a2,a2,a5
   112d4:	d0062483          	lw	s1,-768(a2)
   112d8:	e0ba                	sd	a4,64(sp)
   112da:	4c05                	li	s8,1
   112dc:	0004d763          	bgez	s1,112ea <printf_core+0x144>
   112e0:	6709                	lui	a4,0x2
   112e2:	00ed6d33          	or	s10,s10,a4
   112e6:	409004bb          	negw	s1,s1
   112ea:	6706                	ld	a4,64(sp)
   112ec:	02e00613          	li	a2,46
   112f0:	00074583          	lbu	a1,0(a4) # 2000 <exit-0xe120>
   112f4:	12c59563          	bne	a1,a2,1141e <printf_core+0x278>
   112f8:	00174583          	lbu	a1,1(a4)
   112fc:	02a00613          	li	a2,42
   11300:	10c59563          	bne	a1,a2,1140a <printf_core+0x264>
   11304:	00274603          	lbu	a2,2(a4)
   11308:	45a5                	li	a1,9
   1130a:	fd06051b          	addiw	a0,a2,-48
   1130e:	0ea5e063          	bltu	a1,a0,113ee <printf_core+0x248>
   11312:	00374503          	lbu	a0,3(a4)
   11316:	02400593          	li	a1,36
   1131a:	0cb51a63          	bne	a0,a1,113ee <printf_core+0x248>
   1131e:	060a                	slli	a2,a2,0x2
   11320:	965a                	add	a2,a2,s6
   11322:	45a9                	li	a1,10
   11324:	f4b62023          	sw	a1,-192(a2)
   11328:	00274603          	lbu	a2,2(a4)
   1132c:	6782                	ld	a5,0(sp)
   1132e:	0711                	addi	a4,a4,4
   11330:	0612                	slli	a2,a2,0x4
   11332:	963e                	add	a2,a2,a5
   11334:	d0062a83          	lw	s5,-768(a2)
   11338:	ffface93          	not	t4,s5
   1133c:	e0ba                	sd	a4,64(sp)
   1133e:	01fede9b          	srliw	t4,t4,0x1f
   11342:	4701                	li	a4,0
   11344:	03900f13          	li	t5,57
   11348:	03a00813          	li	a6,58
   1134c:	451d                	li	a0,7
   1134e:	6606                	ld	a2,64(sp)
   11350:	00064583          	lbu	a1,0(a2)
   11354:	fbf5859b          	addiw	a1,a1,-65
   11358:	06bf6063          	bltu	t5,a1,113b8 <printf_core+0x212>
   1135c:	00160593          	addi	a1,a2,1
   11360:	e0ae                	sd	a1,64(sp)
   11362:	00064603          	lbu	a2,0(a2)
   11366:	67c2                	ld	a5,16(sp)
   11368:	fbf6059b          	addiw	a1,a2,-65
   1136c:	02071613          	slli	a2,a4,0x20
   11370:	9201                	srli	a2,a2,0x20
   11372:	03060633          	mul	a2,a2,a6
   11376:	963e                	add	a2,a2,a5
   11378:	962e                	add	a2,a2,a1
   1137a:	00064f83          	lbu	t6,0(a2)
   1137e:	ffff861b          	addiw	a2,t6,-1
   11382:	000f859b          	sext.w	a1,t6
   11386:	08c57f63          	bgeu	a0,a2,11424 <printf_core+0x27e>
   1138a:	c59d                	beqz	a1,113b8 <printf_core+0x212>
   1138c:	466d                	li	a2,27
   1138e:	08c59d63          	bne	a1,a2,11428 <printf_core+0x282>
   11392:	0206d363          	bgez	a3,113b8 <printf_core+0x212>
   11396:	e075                	bnez	s0,1147a <printf_core+0x2d4>
   11398:	bd81                	j	111e8 <printf_core+0x42>
   1139a:	0785                	addi	a5,a5,1
   1139c:	e0be                	sd	a5,64(sp)
   1139e:	56fd                	li	a3,-1
   113a0:	bde1                	j	11278 <printf_core+0xd2>
   113a2:	00ce963b          	sllw	a2,t4,a2
   113a6:	00cd67b3          	or	a5,s10,a2
   113aa:	0705                	addi	a4,a4,1
   113ac:	00078d1b          	sext.w	s10,a5
   113b0:	e0ba                	sd	a4,64(sp)
   113b2:	bdc9                	j	11284 <printf_core+0xde>
   113b4:	000c0863          	beqz	s8,113c4 <printf_core+0x21e>
   113b8:	1a0010ef          	jal	ra,12558 <__errno_location>
   113bc:	47d9                	li	a5,22
   113be:	c11c                	sw	a5,0(a0)
   113c0:	597d                	li	s2,-1
   113c2:	a061                	j	1144a <printf_core+0x2a4>
   113c4:	4481                	li	s1,0
   113c6:	c801                	beqz	s0,113d6 <printf_core+0x230>
   113c8:	000a3603          	ld	a2,0(s4)
   113cc:	4204                	lw	s1,0(a2)
   113ce:	00860593          	addi	a1,a2,8
   113d2:	00ba3023          	sd	a1,0(s4)
   113d6:	0705                	addi	a4,a4,1
   113d8:	e0ba                	sd	a4,64(sp)
   113da:	b709                	j	112dc <printf_core+0x136>
   113dc:	0088                	addi	a0,sp,64
   113de:	f036                	sd	a3,32(sp)
   113e0:	946ff0ef          	jal	ra,10526 <getint>
   113e4:	7682                	ld	a3,32(sp)
   113e6:	84aa                	mv	s1,a0
   113e8:	f00551e3          	bgez	a0,112ea <printf_core+0x144>
   113ec:	ae51                	j	11780 <printf_core+0x5da>
   113ee:	fc0c15e3          	bnez	s8,113b8 <printf_core+0x212>
   113f2:	4a81                	li	s5,0
   113f4:	c809                	beqz	s0,11406 <printf_core+0x260>
   113f6:	000a3603          	ld	a2,0(s4)
   113fa:	00062a83          	lw	s5,0(a2)
   113fe:	00860593          	addi	a1,a2,8
   11402:	00ba3023          	sd	a1,0(s4)
   11406:	0709                	addi	a4,a4,2
   11408:	bf05                	j	11338 <printf_core+0x192>
   1140a:	0705                	addi	a4,a4,1
   1140c:	0088                	addi	a0,sp,64
   1140e:	f036                	sd	a3,32(sp)
   11410:	e0ba                	sd	a4,64(sp)
   11412:	914ff0ef          	jal	ra,10526 <getint>
   11416:	7682                	ld	a3,32(sp)
   11418:	8aaa                	mv	s5,a0
   1141a:	4e85                	li	t4,1
   1141c:	b71d                	j	11342 <printf_core+0x19c>
   1141e:	4e81                	li	t4,0
   11420:	5afd                	li	s5,-1
   11422:	b705                	j	11342 <printf_core+0x19c>
   11424:	872e                	mv	a4,a1
   11426:	b725                	j	1134e <printf_core+0x1a8>
   11428:	0006cf63          	bltz	a3,11446 <printf_core+0x2a0>
   1142c:	6782                	ld	a5,0(sp)
   1142e:	00269613          	slli	a2,a3,0x2
   11432:	0692                	slli	a3,a3,0x4
   11434:	965a                	add	a2,a2,s6
   11436:	96be                	add	a3,a3,a5
   11438:	01f62023          	sw	t6,0(a2)
   1143c:	6290                	ld	a2,0(a3)
   1143e:	6694                	ld	a3,8(a3)
   11440:	e8b2                	sd	a2,80(sp)
   11442:	ecb6                	sd	a3,88(sp)
   11444:	bf89                	j	11396 <printf_core+0x1f0>
   11446:	e015                	bnez	s0,1146a <printf_core+0x2c4>
   11448:	4901                	li	s2,0
   1144a:	60b2                	ld	ra,264(sp)
   1144c:	6412                	ld	s0,256(sp)
   1144e:	74ee                	ld	s1,248(sp)
   11450:	79ae                	ld	s3,232(sp)
   11452:	7a0e                	ld	s4,224(sp)
   11454:	6aee                	ld	s5,216(sp)
   11456:	6b4e                	ld	s6,208(sp)
   11458:	6bae                	ld	s7,200(sp)
   1145a:	6c0e                	ld	s8,192(sp)
   1145c:	7cea                	ld	s9,184(sp)
   1145e:	7d4a                	ld	s10,176(sp)
   11460:	7daa                	ld	s11,168(sp)
   11462:	854a                	mv	a0,s2
   11464:	794e                	ld	s2,240(sp)
   11466:	6151                	addi	sp,sp,272
   11468:	8082                	ret
   1146a:	8652                	mv	a2,s4
   1146c:	0888                	addi	a0,sp,80
   1146e:	f43a                	sd	a4,40(sp)
   11470:	f076                	sd	t4,32(sp)
   11472:	ff1fe0ef          	jal	ra,10462 <pop_arg>
   11476:	7e82                	ld	t4,32(sp)
   11478:	7722                	ld	a4,40(sp)
   1147a:	6686                	ld	a3,64(sp)
   1147c:	fff6c683          	lbu	a3,-1(a3)
   11480:	0006881b          	sext.w	a6,a3
   11484:	c719                	beqz	a4,11492 <printf_core+0x2ec>
   11486:	8abd                	andi	a3,a3,15
   11488:	460d                	li	a2,3
   1148a:	00c69463          	bne	a3,a2,11492 <printf_core+0x2ec>
   1148e:	0df87813          	andi	a6,a6,223
   11492:	032d1793          	slli	a5,s10,0x32
   11496:	0007d663          	bgez	a5,114a2 <printf_core+0x2fc>
   1149a:	76c1                	lui	a3,0xffff0
   1149c:	16fd                	addi	a3,a3,-1
   1149e:	00dd7d33          	and	s10,s10,a3
   114a2:	fbf8069b          	addiw	a3,a6,-65
   114a6:	0006859b          	sext.w	a1,a3
   114aa:	03700613          	li	a2,55
   114ae:	32b66063          	bltu	a2,a1,117ce <printf_core+0x628>
   114b2:	67e2                	ld	a5,24(sp)
   114b4:	1682                	slli	a3,a3,0x20
   114b6:	82f9                	srli	a3,a3,0x1e
   114b8:	96be                	add	a3,a3,a5
   114ba:	4294                	lw	a3,0(a3)
   114bc:	8682                	jr	a3
   114be:	479d                	li	a5,7
   114c0:	2ae7ed63          	bltu	a5,a4,1177a <printf_core+0x5d4>
   114c4:	00271793          	slli	a5,a4,0x2
   114c8:	6755                	lui	a4,0x15
   114ca:	55070713          	addi	a4,a4,1360 # 15550 <__clzdi2+0x1ee>
   114ce:	97ba                	add	a5,a5,a4
   114d0:	439c                	lw	a5,0(a5)
   114d2:	8782                	jr	a5
   114d4:	67c6                	ld	a5,80(sp)
   114d6:	01b7a023          	sw	s11,0(a5)
   114da:	b339                	j	111e8 <printf_core+0x42>
   114dc:	67c6                	ld	a5,80(sp)
   114de:	01279023          	sh	s2,0(a5)
   114e2:	b319                	j	111e8 <printf_core+0x42>
   114e4:	67c6                	ld	a5,80(sp)
   114e6:	01278023          	sb	s2,0(a5)
   114ea:	b9fd                	j	111e8 <printf_core+0x42>
   114ec:	67c6                	ld	a5,80(sp)
   114ee:	0127b023          	sd	s2,0(a5)
   114f2:	b9dd                	j	111e8 <printf_core+0x42>
   114f4:	000a889b          	sext.w	a7,s5
   114f8:	46c1                	li	a3,16
   114fa:	8756                	mv	a4,s5
   114fc:	00d8f363          	bgeu	a7,a3,11502 <printf_core+0x35c>
   11500:	4741                	li	a4,16
   11502:	00070a9b          	sext.w	s5,a4
   11506:	008d6d13          	ori	s10,s10,8
   1150a:	07800813          	li	a6,120
   1150e:	6546                	ld	a0,80(sp)
   11510:	02087693          	andi	a3,a6,32
   11514:	6655                	lui	a2,0x15
   11516:	2681                	sext.w	a3,a3
   11518:	872a                	mv	a4,a0
   1151a:	09f10b93          	addi	s7,sp,159
   1151e:	7c060613          	addi	a2,a2,1984 # 157c0 <xdigits>
   11522:	0e071663          	bnez	a4,1160e <printf_core+0x468>
   11526:	8dce                	mv	s11,s3
   11528:	c909                	beqz	a0,1153a <printf_core+0x394>
   1152a:	008d7713          	andi	a4,s10,8
   1152e:	c711                	beqz	a4,1153a <printf_core+0x394>
   11530:	00485813          	srli	a6,a6,0x4
   11534:	01098db3          	add	s11,s3,a6
   11538:	4c89                	li	s9,2
   1153a:	000e8863          	beqz	t4,1154a <printf_core+0x3a4>
   1153e:	240ac163          	bltz	s5,11780 <printf_core+0x5da>
   11542:	7741                	lui	a4,0xffff0
   11544:	177d                	addi	a4,a4,-1
   11546:	00ed7d33          	and	s10,s10,a4
   1154a:	66c6                	ld	a3,80(sp)
   1154c:	09f10613          	addi	a2,sp,159
   11550:	e299                	bnez	a3,11556 <printf_core+0x3b0>
   11552:	280a8263          	beqz	s5,117d6 <printf_core+0x630>
   11556:	41760733          	sub	a4,a2,s7
   1155a:	0016b693          	seqz	a3,a3
   1155e:	9736                	add	a4,a4,a3
   11560:	01575363          	bge	a4,s5,11566 <printf_core+0x3c0>
   11564:	8756                	mv	a4,s5
   11566:	00070a9b          	sext.w	s5,a4
   1156a:	41760833          	sub	a6,a2,s7
   1156e:	010ad463          	bge	s5,a6,11576 <printf_core+0x3d0>
   11572:	00080a9b          	sext.w	s5,a6
   11576:	80000737          	lui	a4,0x80000
   1157a:	fff74713          	not	a4,a4
   1157e:	4197073b          	subw	a4,a4,s9
   11582:	1f574f63          	blt	a4,s5,11780 <printf_core+0x5da>
   11586:	019a8ebb          	addw	t4,s5,s9
   1158a:	8776                	mv	a4,t4
   1158c:	009ed363          	bge	t4,s1,11592 <printf_core+0x3ec>
   11590:	8726                	mv	a4,s1
   11592:	67a2                	ld	a5,8(sp)
   11594:	f442                	sd	a6,40(sp)
   11596:	0007049b          	sext.w	s1,a4
   1159a:	1e97c363          	blt	a5,s1,11780 <printf_core+0x5da>
   1159e:	86f6                	mv	a3,t4
   115a0:	876a                	mv	a4,s10
   115a2:	8626                	mv	a2,s1
   115a4:	02000593          	li	a1,32
   115a8:	8522                	mv	a0,s0
   115aa:	e476                	sd	t4,8(sp)
   115ac:	fdffe0ef          	jal	ra,1058a <pad>
   115b0:	8666                	mv	a2,s9
   115b2:	85ee                	mv	a1,s11
   115b4:	8522                	mv	a0,s0
   115b6:	fbffe0ef          	jal	ra,10574 <out>
   115ba:	6ea2                	ld	t4,8(sp)
   115bc:	6741                	lui	a4,0x10
   115be:	00ed4733          	xor	a4,s10,a4
   115c2:	86f6                	mv	a3,t4
   115c4:	8626                	mv	a2,s1
   115c6:	03000593          	li	a1,48
   115ca:	8522                	mv	a0,s0
   115cc:	f076                	sd	t4,32(sp)
   115ce:	fbdfe0ef          	jal	ra,1058a <pad>
   115d2:	7822                	ld	a6,40(sp)
   115d4:	4701                	li	a4,0
   115d6:	8656                	mv	a2,s5
   115d8:	0008069b          	sext.w	a3,a6
   115dc:	03000593          	li	a1,48
   115e0:	8522                	mv	a0,s0
   115e2:	e442                	sd	a6,8(sp)
   115e4:	fa7fe0ef          	jal	ra,1058a <pad>
   115e8:	6822                	ld	a6,8(sp)
   115ea:	85de                	mv	a1,s7
   115ec:	8522                	mv	a0,s0
   115ee:	8642                	mv	a2,a6
   115f0:	f85fe0ef          	jal	ra,10574 <out>
   115f4:	7e82                	ld	t4,32(sp)
   115f6:	6709                	lui	a4,0x2
   115f8:	00ed4733          	xor	a4,s10,a4
   115fc:	86f6                	mv	a3,t4
   115fe:	8626                	mv	a2,s1
   11600:	02000593          	li	a1,32
   11604:	8522                	mv	a0,s0
   11606:	f85fe0ef          	jal	ra,1058a <pad>
   1160a:	8ca6                	mv	s9,s1
   1160c:	bef1                	j	111e8 <printf_core+0x42>
   1160e:	00f77593          	andi	a1,a4,15
   11612:	95b2                	add	a1,a1,a2
   11614:	0005c583          	lbu	a1,0(a1)
   11618:	1bfd                	addi	s7,s7,-1
   1161a:	8311                	srli	a4,a4,0x4
   1161c:	8dd5                	or	a1,a1,a3
   1161e:	00bb8023          	sb	a1,0(s7)
   11622:	b701                	j	11522 <printf_core+0x37c>
   11624:	66c6                	ld	a3,80(sp)
   11626:	09f10b93          	addi	s7,sp,159
   1162a:	875e                	mv	a4,s7
   1162c:	ee89                	bnez	a3,11646 <printf_core+0x4a0>
   1162e:	008d7693          	andi	a3,s10,8
   11632:	8dce                	mv	s11,s3
   11634:	f00683e3          	beqz	a3,1153a <printf_core+0x394>
   11638:	41770733          	sub	a4,a4,s7
   1163c:	ef574fe3          	blt	a4,s5,1153a <printf_core+0x394>
   11640:	00170a9b          	addiw	s5,a4,1
   11644:	bddd                	j	1153a <printf_core+0x394>
   11646:	0076f613          	andi	a2,a3,7
   1164a:	1bfd                	addi	s7,s7,-1
   1164c:	0306061b          	addiw	a2,a2,48
   11650:	00cb8023          	sb	a2,0(s7)
   11654:	828d                	srli	a3,a3,0x3
   11656:	bfd9                	j	1162c <printf_core+0x486>
   11658:	6746                	ld	a4,80(sp)
   1165a:	02075063          	bgez	a4,1167a <printf_core+0x4d4>
   1165e:	40e00733          	neg	a4,a4
   11662:	e8ba                	sd	a4,80(sp)
   11664:	4c85                	li	s9,1
   11666:	8dce                	mv	s11,s3
   11668:	6546                	ld	a0,80(sp)
   1166a:	09f10593          	addi	a1,sp,159
   1166e:	f076                	sd	t4,32(sp)
   11670:	e99fe0ef          	jal	ra,10508 <fmt_u>
   11674:	7e82                	ld	t4,32(sp)
   11676:	8baa                	mv	s7,a0
   11678:	b5c9                	j	1153a <printf_core+0x394>
   1167a:	034d1793          	slli	a5,s10,0x34
   1167e:	0007ca63          	bltz	a5,11692 <printf_core+0x4ec>
   11682:	001d7713          	andi	a4,s10,1
   11686:	d365                	beqz	a4,11666 <printf_core+0x4c0>
   11688:	6dd5                	lui	s11,0x15
   1168a:	4c85                	li	s9,1
   1168c:	45ad8d93          	addi	s11,s11,1114 # 1545a <__clzdi2+0xf8>
   11690:	bfe1                	j	11668 <printf_core+0x4c2>
   11692:	6dd5                	lui	s11,0x15
   11694:	4c85                	li	s9,1
   11696:	459d8d93          	addi	s11,s11,1113 # 15459 <__clzdi2+0xf7>
   1169a:	b7f9                	j	11668 <printf_core+0x4c2>
   1169c:	6746                	ld	a4,80(sp)
   1169e:	8dce                	mv	s11,s3
   116a0:	4a85                	li	s5,1
   116a2:	08e10f23          	sb	a4,158(sp)
   116a6:	7741                	lui	a4,0xffff0
   116a8:	177d                	addi	a4,a4,-1
   116aa:	00ed7d33          	and	s10,s10,a4
   116ae:	09f10613          	addi	a2,sp,159
   116b2:	09e10b93          	addi	s7,sp,158
   116b6:	bd55                	j	1156a <printf_core+0x3c4>
   116b8:	6a1000ef          	jal	ra,12558 <__errno_location>
   116bc:	4108                	lw	a0,0(a0)
   116be:	6c7000ef          	jal	ra,12584 <strerror>
   116c2:	8baa                	mv	s7,a0
   116c4:	85d6                	mv	a1,s5
   116c6:	000ad663          	bgez	s5,116d2 <printf_core+0x52c>
   116ca:	800005b7          	lui	a1,0x80000
   116ce:	fff5c593          	not	a1,a1
   116d2:	855e                	mv	a0,s7
   116d4:	314000ef          	jal	ra,119e8 <strnlen>
   116d8:	00ab8633          	add	a2,s7,a0
   116dc:	000ad563          	bgez	s5,116e6 <printf_core+0x540>
   116e0:	00064703          	lbu	a4,0(a2)
   116e4:	ef51                	bnez	a4,11780 <printf_core+0x5da>
   116e6:	7741                	lui	a4,0xffff0
   116e8:	177d                	addi	a4,a4,-1
   116ea:	00050a9b          	sext.w	s5,a0
   116ee:	00ed7d33          	and	s10,s10,a4
   116f2:	8dce                	mv	s11,s3
   116f4:	bd9d                	j	1156a <printf_core+0x3c4>
   116f6:	6bc6                	ld	s7,80(sp)
   116f8:	fc0b96e3          	bnez	s7,116c4 <printf_core+0x51e>
   116fc:	6e55                	lui	t3,0x15
   116fe:	468e0b93          	addi	s7,t3,1128 # 15468 <__clzdi2+0x106>
   11702:	b7c9                	j	116c4 <printf_core+0x51e>
   11704:	6746                	ld	a4,80(sp)
   11706:	c682                	sw	zero,76(sp)
   11708:	5afd                	li	s5,-1
   1170a:	c4ba                	sw	a4,72(sp)
   1170c:	00b8                	addi	a4,sp,72
   1170e:	e8ba                	sd	a4,80(sp)
   11710:	6dc6                	ld	s11,80(sp)
   11712:	4c81                	li	s9,0
   11714:	015cff63          	bgeu	s9,s5,11732 <printf_core+0x58c>
   11718:	000da583          	lw	a1,0(s11)
   1171c:	c999                	beqz	a1,11732 <printf_core+0x58c>
   1171e:	1828                	addi	a0,sp,56
   11720:	0d91                	addi	s11,s11,4
   11722:	01a010ef          	jal	ra,1273c <wctomb>
   11726:	c8054de3          	bltz	a0,113c0 <printf_core+0x21a>
   1172a:	419a8733          	sub	a4,s5,s9
   1172e:	04a77e63          	bgeu	a4,a0,1178a <printf_core+0x5e4>
   11732:	80000737          	lui	a4,0x80000
   11736:	fff74713          	not	a4,a4
   1173a:	05976363          	bltu	a4,s9,11780 <printf_core+0x5da>
   1173e:	000c8d9b          	sext.w	s11,s9
   11742:	876a                	mv	a4,s10
   11744:	86ee                	mv	a3,s11
   11746:	8626                	mv	a2,s1
   11748:	02000593          	li	a1,32
   1174c:	8522                	mv	a0,s0
   1174e:	e3dfe0ef          	jal	ra,1058a <pad>
   11752:	6bc6                	ld	s7,80(sp)
   11754:	4a81                	li	s5,0
   11756:	039aec63          	bltu	s5,s9,1178e <printf_core+0x5e8>
   1175a:	6709                	lui	a4,0x2
   1175c:	00ed4733          	xor	a4,s10,a4
   11760:	86ee                	mv	a3,s11
   11762:	8626                	mv	a2,s1
   11764:	02000593          	li	a1,32
   11768:	8522                	mv	a0,s0
   1176a:	e21fe0ef          	jal	ra,1058a <pad>
   1176e:	8326                	mv	t1,s1
   11770:	01b4d363          	bge	s1,s11,11776 <printf_core+0x5d0>
   11774:	836e                	mv	t1,s11
   11776:	00030c9b          	sext.w	s9,t1
   1177a:	67a2                	ld	a5,8(sp)
   1177c:	a797d6e3          	bge	a5,s9,111e8 <printf_core+0x42>
   11780:	5d9000ef          	jal	ra,12558 <__errno_location>
   11784:	04b00793          	li	a5,75
   11788:	b91d                	j	113be <printf_core+0x218>
   1178a:	9caa                	add	s9,s9,a0
   1178c:	b761                	j	11714 <printf_core+0x56e>
   1178e:	000ba583          	lw	a1,0(s7)
   11792:	d5e1                	beqz	a1,1175a <printf_core+0x5b4>
   11794:	1828                	addi	a0,sp,56
   11796:	7a7000ef          	jal	ra,1273c <wctomb>
   1179a:	9aaa                	add	s5,s5,a0
   1179c:	0b91                	addi	s7,s7,4
   1179e:	862a                	mv	a2,a0
   117a0:	fb5cede3          	bltu	s9,s5,1175a <printf_core+0x5b4>
   117a4:	182c                	addi	a1,sp,56
   117a6:	8522                	mv	a0,s0
   117a8:	dcdfe0ef          	jal	ra,10574 <out>
   117ac:	b76d                	j	11756 <printf_core+0x5b0>
   117ae:	000e8463          	beqz	t4,117b6 <printf_core+0x610>
   117b2:	fc0ac7e3          	bltz	s5,11780 <printf_core+0x5da>
   117b6:	65c6                	ld	a1,80(sp)
   117b8:	6666                	ld	a2,88(sp)
   117ba:	87ea                	mv	a5,s10
   117bc:	8756                	mv	a4,s5
   117be:	86a6                	mv	a3,s1
   117c0:	8522                	mv	a0,s0
   117c2:	e3dfe0ef          	jal	ra,105fe <fmt_fp>
   117c6:	8caa                	mv	s9,a0
   117c8:	fa0559e3          	bgez	a0,1177a <printf_core+0x5d4>
   117cc:	bf55                	j	11780 <printf_core+0x5da>
   117ce:	8dce                	mv	s11,s3
   117d0:	09f10613          	addi	a2,sp,159
   117d4:	bb59                	j	1156a <printf_core+0x3c4>
   117d6:	8bb2                	mv	s7,a2
   117d8:	bb49                	j	1156a <printf_core+0x3c4>
   117da:	c60418e3          	bnez	s0,1144a <printf_core+0x2a4>
   117de:	c60c05e3          	beqz	s8,11448 <printf_core+0x2a2>
   117e2:	4405                	li	s0,1
   117e4:	44a9                	li	s1,10
   117e6:	00241793          	slli	a5,s0,0x2
   117ea:	97da                	add	a5,a5,s6
   117ec:	438c                	lw	a1,0(a5)
   117ee:	ed81                	bnez	a1,11806 <printf_core+0x660>
   117f0:	4729                	li	a4,10
   117f2:	00241793          	slli	a5,s0,0x2
   117f6:	97da                	add	a5,a5,s6
   117f8:	439c                	lw	a5,0(a5)
   117fa:	ba079fe3          	bnez	a5,113b8 <printf_core+0x212>
   117fe:	0405                	addi	s0,s0,1
   11800:	fee419e3          	bne	s0,a4,117f2 <printf_core+0x64c>
   11804:	a819                	j	1181a <printf_core+0x674>
   11806:	6782                	ld	a5,0(sp)
   11808:	00441513          	slli	a0,s0,0x4
   1180c:	8652                	mv	a2,s4
   1180e:	953e                	add	a0,a0,a5
   11810:	0405                	addi	s0,s0,1
   11812:	c51fe0ef          	jal	ra,10462 <pop_arg>
   11816:	fc9418e3          	bne	s0,s1,117e6 <printf_core+0x640>
   1181a:	4905                	li	s2,1
   1181c:	b13d                	j	1144a <printf_core+0x2a4>

000000000001181e <vfprintf>:
   1181e:	710d                	addi	sp,sp,-352
   11820:	eaa2                	sd	s0,336(sp)
   11822:	e032                	sd	a2,0(sp)
   11824:	842a                	mv	s0,a0
   11826:	0038                	addi	a4,sp,8
   11828:	0114                	addi	a3,sp,128
   1182a:	860a                	mv	a2,sp
   1182c:	4501                	li	a0,0
   1182e:	e6a6                	sd	s1,328(sp)
   11830:	f656                	sd	s5,296(sp)
   11832:	ee86                	sd	ra,344(sp)
   11834:	e2ca                	sd	s2,320(sp)
   11836:	fe4e                	sd	s3,312(sp)
   11838:	fa52                	sd	s4,304(sp)
   1183a:	8aae                	mv	s5,a1
   1183c:	e402                	sd	zero,8(sp)
   1183e:	e802                	sd	zero,16(sp)
   11840:	ec02                	sd	zero,24(sp)
   11842:	f002                	sd	zero,32(sp)
   11844:	f402                	sd	zero,40(sp)
   11846:	961ff0ef          	jal	ra,111a6 <printf_core>
   1184a:	54fd                	li	s1,-1
   1184c:	0a054663          	bltz	a0,118f8 <vfprintf+0xda>
   11850:	08c42783          	lw	a5,140(s0)
   11854:	4a01                	li	s4,0
   11856:	02079713          	slli	a4,a5,0x20
   1185a:	00074663          	bltz	a4,11866 <vfprintf+0x48>
   1185e:	8522                	mv	a0,s0
   11860:	733000ef          	jal	ra,12792 <__lockfile>
   11864:	8a2a                	mv	s4,a0
   11866:	401c                	lw	a5,0(s0)
   11868:	08842703          	lw	a4,136(s0)
   1186c:	0207f913          	andi	s2,a5,32
   11870:	2901                	sext.w	s2,s2
   11872:	00e04563          	bgtz	a4,1187c <vfprintf+0x5e>
   11876:	fdf7f793          	andi	a5,a5,-33
   1187a:	c01c                	sw	a5,0(s0)
   1187c:	703c                	ld	a5,96(s0)
   1187e:	e7d9                	bnez	a5,1190c <vfprintf+0xee>
   11880:	181c                	addi	a5,sp,48
   11882:	05843983          	ld	s3,88(s0)
   11886:	ec3c                	sd	a5,88(s0)
   11888:	05000793          	li	a5,80
   1188c:	f03c                	sd	a5,96(s0)
   1188e:	02043023          	sd	zero,32(s0)
   11892:	02043c23          	sd	zero,56(s0)
   11896:	02043423          	sd	zero,40(s0)
   1189a:	8522                	mv	a0,s0
   1189c:	044010ef          	jal	ra,128e0 <__towrite>
   118a0:	54fd                	li	s1,-1
   118a2:	e909                	bnez	a0,118b4 <vfprintf+0x96>
   118a4:	0038                	addi	a4,sp,8
   118a6:	0114                	addi	a3,sp,128
   118a8:	860a                	mv	a2,sp
   118aa:	85d6                	mv	a1,s5
   118ac:	8522                	mv	a0,s0
   118ae:	8f9ff0ef          	jal	ra,111a6 <printf_core>
   118b2:	84aa                	mv	s1,a0
   118b4:	02098463          	beqz	s3,118dc <vfprintf+0xbe>
   118b8:	643c                	ld	a5,72(s0)
   118ba:	4601                	li	a2,0
   118bc:	4581                	li	a1,0
   118be:	8522                	mv	a0,s0
   118c0:	9782                	jalr	a5
   118c2:	741c                	ld	a5,40(s0)
   118c4:	e391                	bnez	a5,118c8 <vfprintf+0xaa>
   118c6:	54fd                	li	s1,-1
   118c8:	05343c23          	sd	s3,88(s0)
   118cc:	06043023          	sd	zero,96(s0)
   118d0:	02043023          	sd	zero,32(s0)
   118d4:	02043c23          	sd	zero,56(s0)
   118d8:	02043423          	sd	zero,40(s0)
   118dc:	401c                	lw	a5,0(s0)
   118de:	0207f713          	andi	a4,a5,32
   118e2:	c311                	beqz	a4,118e6 <vfprintf+0xc8>
   118e4:	54fd                	li	s1,-1
   118e6:	00f96933          	or	s2,s2,a5
   118ea:	01242023          	sw	s2,0(s0)
   118ee:	000a0563          	beqz	s4,118f8 <vfprintf+0xda>
   118f2:	8522                	mv	a0,s0
   118f4:	73b000ef          	jal	ra,1282e <__unlockfile>
   118f8:	60f6                	ld	ra,344(sp)
   118fa:	6456                	ld	s0,336(sp)
   118fc:	6916                	ld	s2,320(sp)
   118fe:	79f2                	ld	s3,312(sp)
   11900:	7a52                	ld	s4,304(sp)
   11902:	7ab2                	ld	s5,296(sp)
   11904:	8526                	mv	a0,s1
   11906:	64b6                	ld	s1,328(sp)
   11908:	6135                	addi	sp,sp,352
   1190a:	8082                	ret
   1190c:	02043983          	ld	s3,32(s0)
   11910:	f80985e3          	beqz	s3,1189a <vfprintf+0x7c>
   11914:	4981                	li	s3,0
   11916:	b779                	j	118a4 <vfprintf+0x86>

0000000000011918 <memset>:
   11918:	c679                	beqz	a2,119e6 <memset+0xce>
   1191a:	0ff5f793          	zext.b	a5,a1
   1191e:	00f50023          	sb	a5,0(a0) # 13000 <__unlock+0x20>
   11922:	00c50733          	add	a4,a0,a2
   11926:	fef70fa3          	sb	a5,-1(a4) # 1fff <exit-0xe121>
   1192a:	4689                	li	a3,2
   1192c:	0ac6fd63          	bgeu	a3,a2,119e6 <memset+0xce>
   11930:	00f500a3          	sb	a5,1(a0)
   11934:	00f50123          	sb	a5,2(a0)
   11938:	fef70f23          	sb	a5,-2(a4)
   1193c:	fef70ea3          	sb	a5,-3(a4)
   11940:	4699                	li	a3,6
   11942:	0ac6f263          	bgeu	a3,a2,119e6 <memset+0xce>
   11946:	00f501a3          	sb	a5,3(a0)
   1194a:	fef70e23          	sb	a5,-4(a4)
   1194e:	46a1                	li	a3,8
   11950:	08c6fb63          	bgeu	a3,a2,119e6 <memset+0xce>
   11954:	0ff5f593          	zext.b	a1,a1
   11958:	0085979b          	slliw	a5,a1,0x8
   1195c:	40a00733          	neg	a4,a0
   11960:	8b0d                	andi	a4,a4,3
   11962:	9dbd                	addw	a1,a1,a5
   11964:	8e19                	sub	a2,a2,a4
   11966:	0105979b          	slliw	a5,a1,0x10
   1196a:	9dbd                	addw	a1,a1,a5
   1196c:	972a                	add	a4,a4,a0
   1196e:	9a71                	andi	a2,a2,-4
   11970:	c30c                	sw	a1,0(a4)
   11972:	00c707b3          	add	a5,a4,a2
   11976:	feb7ae23          	sw	a1,-4(a5)
   1197a:	06c6f663          	bgeu	a3,a2,119e6 <memset+0xce>
   1197e:	c34c                	sw	a1,4(a4)
   11980:	c70c                	sw	a1,8(a4)
   11982:	feb7aa23          	sw	a1,-12(a5)
   11986:	feb7ac23          	sw	a1,-8(a5)
   1198a:	46e1                	li	a3,24
   1198c:	04c6fd63          	bgeu	a3,a2,119e6 <memset+0xce>
   11990:	00477813          	andi	a6,a4,4
   11994:	c74c                	sw	a1,12(a4)
   11996:	cb0c                	sw	a1,16(a4)
   11998:	cb4c                	sw	a1,20(a4)
   1199a:	cf0c                	sw	a1,24(a4)
   1199c:	0861                	addi	a6,a6,24
   1199e:	02059893          	slli	a7,a1,0x20
   119a2:	0208d893          	srli	a7,a7,0x20
   119a6:	02059693          	slli	a3,a1,0x20
   119aa:	feb7a223          	sw	a1,-28(a5)
   119ae:	feb7a423          	sw	a1,-24(a5)
   119b2:	feb7a623          	sw	a1,-20(a5)
   119b6:	feb7a823          	sw	a1,-16(a5)
   119ba:	41060633          	sub	a2,a2,a6
   119be:	47fd                	li	a5,31
   119c0:	9742                	add	a4,a4,a6
   119c2:	0116e6b3          	or	a3,a3,a7
   119c6:	02c7f063          	bgeu	a5,a2,119e6 <memset+0xce>
   119ca:	1601                	addi	a2,a2,-32
   119cc:	fe067793          	andi	a5,a2,-32
   119d0:	02078793          	addi	a5,a5,32
   119d4:	97ba                	add	a5,a5,a4
   119d6:	e314                	sd	a3,0(a4)
   119d8:	e714                	sd	a3,8(a4)
   119da:	eb14                	sd	a3,16(a4)
   119dc:	ef14                	sd	a3,24(a4)
   119de:	02070713          	addi	a4,a4,32
   119e2:	fef71ae3          	bne	a4,a5,119d6 <memset+0xbe>
   119e6:	8082                	ret

00000000000119e8 <strnlen>:
   119e8:	1101                	addi	sp,sp,-32
   119ea:	e822                	sd	s0,16(sp)
   119ec:	862e                	mv	a2,a1
   119ee:	842e                	mv	s0,a1
   119f0:	4581                	li	a1,0
   119f2:	e426                	sd	s1,8(sp)
   119f4:	ec06                	sd	ra,24(sp)
   119f6:	84aa                	mv	s1,a0
   119f8:	046010ef          	jal	ra,12a3e <memchr>
   119fc:	c519                	beqz	a0,11a0a <strnlen+0x22>
   119fe:	60e2                	ld	ra,24(sp)
   11a00:	6442                	ld	s0,16(sp)
   11a02:	8d05                	sub	a0,a0,s1
   11a04:	64a2                	ld	s1,8(sp)
   11a06:	6105                	addi	sp,sp,32
   11a08:	8082                	ret
   11a0a:	60e2                	ld	ra,24(sp)
   11a0c:	8522                	mv	a0,s0
   11a0e:	6442                	ld	s0,16(sp)
   11a10:	64a2                	ld	s1,8(sp)
   11a12:	6105                	addi	sp,sp,32
   11a14:	8082                	ret

0000000000011a16 <init_file_lock>:
   11a16:	c909                	beqz	a0,11a28 <init_file_lock+0x12>
   11a18:	08c52783          	lw	a5,140(a0)
   11a1c:	02079713          	slli	a4,a5,0x20
   11a20:	00075463          	bgez	a4,11a28 <init_file_lock+0x12>
   11a24:	08052623          	sw	zero,140(a0)
   11a28:	8082                	ret

0000000000011a2a <__wake.constprop.0>:
   11a2a:	87aa                	mv	a5,a0
   11a2c:	c199                	beqz	a1,11a32 <__wake.constprop.0+0x8>
   11a2e:	08000593          	li	a1,128
   11a32:	06200893          	li	a7,98
   11a36:	853e                	mv	a0,a5
   11a38:	0015e593          	ori	a1,a1,1
   11a3c:	4605                	li	a2,1
   11a3e:	00000073          	ecall
   11a42:	fda00713          	li	a4,-38
   11a46:	00e51663          	bne	a0,a4,11a52 <__wake.constprop.0+0x28>
   11a4a:	853e                	mv	a0,a5
   11a4c:	4585                	li	a1,1
   11a4e:	00000073          	ecall
   11a52:	8082                	ret

0000000000011a54 <__acquire_ptc>:
   11a54:	8082                	ret

0000000000011a56 <__tl_lock>:
   11a56:	7179                	addi	sp,sp,-48
   11a58:	f406                	sd	ra,40(sp)
   11a5a:	f022                	sd	s0,32(sp)
   11a5c:	ec26                	sd	s1,24(sp)
   11a5e:	e84a                	sd	s2,16(sp)
   11a60:	e44e                	sd	s3,8(sp)
   11a62:	8792                	mv	a5,tp
   11a64:	f587a483          	lw	s1,-168(a5)
   11a68:	fc81a783          	lw	a5,-56(gp) # 187c8 <__thread_list_lock>
   11a6c:	2781                	sext.w	a5,a5
   11a6e:	02f48563          	beq	s1,a5,11a98 <__tl_lock+0x42>
   11a72:	fc818413          	addi	s0,gp,-56 # 187c8 <__thread_list_lock>
   11a76:	4901                	li	s2,0
   11a78:	1604262f          	lr.w.aqrl	a2,(s0)
   11a7c:	01261563          	bne	a2,s2,11a86 <__tl_lock+0x30>
   11a80:	1e9427af          	sc.w.aqrl	a5,s1,(s0)
   11a84:	fbf5                	bnez	a5,11a78 <__tl_lock+0x22>
   11a86:	2601                	sext.w	a2,a2
   11a88:	ce09                	beqz	a2,11aa2 <__tl_lock+0x4c>
   11a8a:	4681                	li	a3,0
   11a8c:	96418593          	addi	a1,gp,-1692 # 18164 <tl_lock_waiters>
   11a90:	8522                	mv	a0,s0
   11a92:	70e010ef          	jal	ra,131a0 <__wait>
   11a96:	b7cd                	j	11a78 <__tl_lock+0x22>
   11a98:	96018793          	addi	a5,gp,-1696 # 18160 <tl_lock_count>
   11a9c:	4398                	lw	a4,0(a5)
   11a9e:	2705                	addiw	a4,a4,1
   11aa0:	c398                	sw	a4,0(a5)
   11aa2:	70a2                	ld	ra,40(sp)
   11aa4:	7402                	ld	s0,32(sp)
   11aa6:	64e2                	ld	s1,24(sp)
   11aa8:	6942                	ld	s2,16(sp)
   11aaa:	69a2                	ld	s3,8(sp)
   11aac:	6145                	addi	sp,sp,48
   11aae:	8082                	ret

0000000000011ab0 <__tl_unlock>:
   11ab0:	96018793          	addi	a5,gp,-1696 # 18160 <tl_lock_count>
   11ab4:	4398                	lw	a4,0(a5)
   11ab6:	c701                	beqz	a4,11abe <__tl_unlock+0xe>
   11ab8:	377d                	addiw	a4,a4,-1
   11aba:	c398                	sw	a4,0(a5)
   11abc:	8082                	ret
   11abe:	0330000f          	fence	rw,rw
   11ac2:	fc01a423          	sw	zero,-56(gp) # 187c8 <__thread_list_lock>
   11ac6:	0330000f          	fence	rw,rw
   11aca:	9641a783          	lw	a5,-1692(gp) # 18164 <tl_lock_waiters>
   11ace:	c789                	beqz	a5,11ad8 <__tl_unlock+0x28>
   11ad0:	4581                	li	a1,0
   11ad2:	fc818513          	addi	a0,gp,-56 # 187c8 <__thread_list_lock>
   11ad6:	bf91                	j	11a2a <__wake.constprop.0>
   11ad8:	8082                	ret

0000000000011ada <__tl_sync>:
   11ada:	1101                	addi	sp,sp,-32
   11adc:	ec06                	sd	ra,24(sp)
   11ade:	e822                	sd	s0,16(sp)
   11ae0:	e426                	sd	s1,8(sp)
   11ae2:	0330000f          	fence	rw,rw
   11ae6:	fc81a603          	lw	a2,-56(gp) # 187c8 <__thread_list_lock>
   11aea:	2601                	sext.w	a2,a2
   11aec:	c605                	beqz	a2,11b14 <__tl_sync+0x3a>
   11aee:	96418493          	addi	s1,gp,-1692 # 18164 <tl_lock_waiters>
   11af2:	4681                	li	a3,0
   11af4:	96418593          	addi	a1,gp,-1692 # 18164 <tl_lock_waiters>
   11af8:	fc818513          	addi	a0,gp,-56 # 187c8 <__thread_list_lock>
   11afc:	6a4010ef          	jal	ra,131a0 <__wait>
   11b00:	409c                	lw	a5,0(s1)
   11b02:	cb89                	beqz	a5,11b14 <__tl_sync+0x3a>
   11b04:	fc818513          	addi	a0,gp,-56 # 187c8 <__thread_list_lock>
   11b08:	6442                	ld	s0,16(sp)
   11b0a:	60e2                	ld	ra,24(sp)
   11b0c:	64a2                	ld	s1,8(sp)
   11b0e:	4581                	li	a1,0
   11b10:	6105                	addi	sp,sp,32
   11b12:	bf21                	j	11a2a <__wake.constprop.0>
   11b14:	60e2                	ld	ra,24(sp)
   11b16:	6442                	ld	s0,16(sp)
   11b18:	64a2                	ld	s1,8(sp)
   11b1a:	6105                	addi	sp,sp,32
   11b1c:	8082                	ret

0000000000011b1e <__pthread_exit>:
   11b1e:	7131                	addi	sp,sp,-192
   11b20:	fd06                	sd	ra,184(sp)
   11b22:	f922                	sd	s0,176(sp)
   11b24:	f526                	sd	s1,168(sp)
   11b26:	f14a                	sd	s2,160(sp)
   11b28:	ed4e                	sd	s3,152(sp)
   11b2a:	e952                	sd	s4,144(sp)
   11b2c:	e556                	sd	s5,136(sp)
   11b2e:	8412                	mv	s0,tp
   11b30:	4785                	li	a5,1
   11b32:	f6f40423          	sb	a5,-152(s0)
   11b36:	f3840413          	addi	s0,s0,-200
   11b3a:	020408a3          	sb	zero,49(s0)
   11b3e:	f028                	sd	a0,96(s0)
   11b40:	743c                	ld	a5,104(s0)
   11b42:	e3ad                	bnez	a5,11ba4 <__pthread_exit+0x86>
   11b44:	f11ff0ef          	jal	ra,11a54 <__acquire_ptc>
   11b48:	850a                	mv	a0,sp
   11b4a:	421000ef          	jal	ra,1276a <__block_app_sigs>
   11b4e:	02840913          	addi	s2,s0,40
   11b52:	4789                	li	a5,2
   11b54:	4705                	li	a4,1
   11b56:	16092a2f          	lr.w.aqrl	s4,(s2)
   11b5a:	00fa1563          	bne	s4,a5,11b64 <__pthread_exit+0x46>
   11b5e:	1ee926af          	sc.w.aqrl	a3,a4,(s2)
   11b62:	faf5                	bnez	a3,11b56 <__pthread_exit+0x38>
   11b64:	478d                	li	a5,3
   11b66:	000a099b          	sext.w	s3,s4
   11b6a:	00f99663          	bne	s3,a5,11b76 <__pthread_exit+0x58>
   11b6e:	7c1c                	ld	a5,56(s0)
   11b70:	c399                	beqz	a5,11b76 <__pthread_exit+0x58>
   11b72:	71a000ef          	jal	ra,1228c <__vm_wait>
   11b76:	0a040493          	addi	s1,s0,160
   11b7a:	8526                	mv	a0,s1
   11b7c:	38c010ef          	jal	ra,12f08 <__lock>
   11b80:	ed7ff0ef          	jal	ra,11a56 <__tl_lock>
   11b84:	681c                	ld	a5,16(s0)
   11b86:	02879563          	bne	a5,s0,11bb0 <__pthread_exit+0x92>
   11b8a:	f27ff0ef          	jal	ra,11ab0 <__tl_unlock>
   11b8e:	8526                	mv	a0,s1
   11b90:	450010ef          	jal	ra,12fe0 <__unlock>
   11b94:	850a                	mv	a0,sp
   11b96:	03442423          	sw	s4,40(s0)
   11b9a:	3e7000ef          	jal	ra,12780 <__restore_sigs>
   11b9e:	4501                	li	a0,0
   11ba0:	d80fe0ef          	jal	ra,10120 <exit>
   11ba4:	6398                	ld	a4,0(a5)
   11ba6:	6788                	ld	a0,8(a5)
   11ba8:	6b9c                	ld	a5,16(a5)
   11baa:	f43c                	sd	a5,104(s0)
   11bac:	9702                	jalr	a4
   11bae:	bf49                	j	11b40 <__pthread_exit+0x22>
   11bb0:	708000ef          	jal	ra,122b8 <__vm_lock>
   11bb4:	07840a93          	addi	s5,s0,120
   11bb8:	40000a37          	lui	s4,0x40000
   11bbc:	7c3c                	ld	a5,120(s0)
   11bbe:	c399                	beqz	a5,11bc4 <__pthread_exit+0xa6>
   11bc0:	09579263          	bne	a5,s5,11c44 <__pthread_exit+0x126>
   11bc4:	718000ef          	jal	ra,122dc <__vm_unlock>
   11bc8:	e8dff0ef          	jal	ra,11a54 <__acquire_ptc>
   11bcc:	e89ff0ef          	jal	ra,11a54 <__acquire_ptc>
   11bd0:	f6018793          	addi	a5,gp,-160 # 18760 <__libc>
   11bd4:	43d8                	lw	a4,4(a5)
   11bd6:	fff7069b          	addiw	a3,a4,-1
   11bda:	c3d4                	sw	a3,4(a5)
   11bdc:	e681                	bnez	a3,11be4 <__pthread_exit+0xc6>
   11bde:	577d                	li	a4,-1
   11be0:	00e781a3          	sb	a4,3(a5)
   11be4:	681c                	ld	a5,16(s0)
   11be6:	6418                	ld	a4,8(s0)
   11be8:	e798                	sd	a4,8(a5)
   11bea:	6418                	ld	a4,8(s0)
   11bec:	eb1c                	sd	a5,16(a4)
   11bee:	e800                	sd	s0,16(s0)
   11bf0:	e400                	sd	s0,8(s0)
   11bf2:	478d                	li	a5,3
   11bf4:	02f99363          	bne	s3,a5,11c1a <__pthread_exit+0xfc>
   11bf8:	7c1c                	ld	a5,56(s0)
   11bfa:	c385                	beqz	a5,11c1a <__pthread_exit+0xfc>
   11bfc:	850a                	mv	a0,sp
   11bfe:	357000ef          	jal	ra,12754 <__block_all_sigs>
   11c02:	605c                	ld	a5,128(s0)
   11c04:	c799                	beqz	a5,11c12 <__pthread_exit+0xf4>
   11c06:	06300893          	li	a7,99
   11c0a:	4501                	li	a0,0
   11c0c:	45e1                	li	a1,24
   11c0e:	00000073          	ecall
   11c12:	602c                	ld	a1,64(s0)
   11c14:	7c08                	ld	a0,56(s0)
   11c16:	63e000ef          	jal	ra,12254 <__unmapself>
   11c1a:	0330000f          	fence	rw,rw
   11c1e:	02042423          	sw	zero,40(s0)
   11c22:	0330000f          	fence	rw,rw
   11c26:	4585                	li	a1,1
   11c28:	854a                	mv	a0,s2
   11c2a:	e01ff0ef          	jal	ra,11a2a <__wake.constprop.0>
   11c2e:	8526                	mv	a0,s1
   11c30:	02042023          	sw	zero,32(s0)
   11c34:	3ac010ef          	jal	ra,12fe0 <__unlock>
   11c38:	05d00893          	li	a7,93
   11c3c:	4501                	li	a0,0
   11c3e:	00000073          	ecall
   11c42:	bfdd                	j	11c38 <__pthread_exit+0x11a>
   11c44:	fe87a683          	lw	a3,-24(a5)
   11c48:	fe07a583          	lw	a1,-32(a5)
   11c4c:	e45c                	sd	a5,136(s0)
   11c4e:	6398                	ld	a4,0(a5)
   11c50:	2681                	sext.w	a3,a3
   11c52:	fe478513          	addi	a0,a5,-28
   11c56:	fc38                	sd	a4,120(s0)
   11c58:	fe47a703          	lw	a4,-28(a5)
   11c5c:	2701                	sext.w	a4,a4
   11c5e:	1605262f          	lr.w.aqrl	a2,(a0)
   11c62:	00e61663          	bne	a2,a4,11c6e <__pthread_exit+0x150>
   11c66:	1f45282f          	sc.w.aqrl	a6,s4,(a0)
   11c6a:	fe081ae3          	bnez	a6,11c5e <__pthread_exit+0x140>
   11c6e:	2601                	sext.w	a2,a2
   11c70:	fec714e3          	bne	a4,a2,11c58 <__pthread_exit+0x13a>
   11c74:	08043423          	sd	zero,136(s0)
   11c78:	00074363          	bltz	a4,11c7e <__pthread_exit+0x160>
   11c7c:	d2a1                	beqz	a3,11bbc <__pthread_exit+0x9e>
   11c7e:	fff5c593          	not	a1,a1
   11c82:	0805f593          	andi	a1,a1,128
   11c86:	da5ff0ef          	jal	ra,11a2a <__wake.constprop.0>
   11c8a:	bf0d                	j	11bbc <__pthread_exit+0x9e>

0000000000011c8c <start>:
   11c8c:	491c                	lw	a5,16(a0)
   11c8e:	1101                	addi	sp,sp,-32
   11c90:	e822                	sd	s0,16(sp)
   11c92:	ec06                	sd	ra,24(sp)
   11c94:	2781                	sext.w	a5,a5
   11c96:	842a                	mv	s0,a0
   11c98:	c3a1                	beqz	a5,11cd8 <start+0x4c>
   11c9a:	0541                	addi	a0,a0,16
   11c9c:	4785                	li	a5,1
   11c9e:	4689                	li	a3,2
   11ca0:	1605272f          	lr.w.aqrl	a4,(a0)
   11ca4:	00f71563          	bne	a4,a5,11cae <start+0x22>
   11ca8:	1ed5262f          	sc.w.aqrl	a2,a3,(a0)
   11cac:	fa75                	bnez	a2,11ca0 <start+0x14>
   11cae:	2701                	sext.w	a4,a4
   11cb0:	00f71963          	bne	a4,a5,11cc2 <start+0x36>
   11cb4:	4685                	li	a3,1
   11cb6:	4609                	li	a2,2
   11cb8:	4581                	li	a1,0
   11cba:	e42a                	sd	a0,8(sp)
   11cbc:	4e4010ef          	jal	ra,131a0 <__wait>
   11cc0:	6522                	ld	a0,8(sp)
   11cc2:	481c                	lw	a5,16(s0)
   11cc4:	2781                	sext.w	a5,a5
   11cc6:	cb89                	beqz	a5,11cd8 <start+0x4c>
   11cc8:	06000893          	li	a7,96
   11ccc:	00000073          	ecall
   11cd0:	05d00893          	li	a7,93
   11cd4:	4501                	li	a0,0
   11cd6:	bfdd                	j	11ccc <start+0x40>
   11cd8:	08700893          	li	a7,135
   11cdc:	4509                	li	a0,2
   11cde:	01840593          	addi	a1,s0,24
   11ce2:	4601                	li	a2,0
   11ce4:	46a1                	li	a3,8
   11ce6:	00000073          	ecall
   11cea:	601c                	ld	a5,0(s0)
   11cec:	6408                	ld	a0,8(s0)
   11cee:	9782                	jalr	a5
   11cf0:	e2fff0ef          	jal	ra,11b1e <__pthread_exit>

0000000000011cf4 <start_c11>:
   11cf4:	611c                	ld	a5,0(a0)
   11cf6:	6508                	ld	a0,8(a0)
   11cf8:	1141                	addi	sp,sp,-16
   11cfa:	e406                	sd	ra,8(sp)
   11cfc:	9782                	jalr	a5
   11cfe:	e21ff0ef          	jal	ra,11b1e <__pthread_exit>

0000000000011d02 <__do_cleanup_push>:
   11d02:	8792                	mv	a5,tp
   11d04:	fa07b703          	ld	a4,-96(a5)
   11d08:	e918                	sd	a4,16(a0)
   11d0a:	faa7b023          	sd	a0,-96(a5)
   11d0e:	8082                	ret

0000000000011d10 <__do_cleanup_pop>:
   11d10:	8792                	mv	a5,tp
   11d12:	6918                	ld	a4,16(a0)
   11d14:	fae7b023          	sd	a4,-96(a5)
   11d18:	8082                	ret

0000000000011d1a <__pthread_create>:
   11d1a:	7129                	addi	sp,sp,-320
   11d1c:	f626                	sd	s1,296(sp)
   11d1e:	e25a                	sd	s6,256(sp)
   11d20:	e032                	sd	a2,0(sp)
   11d22:	8b2a                	mv	s6,a0
   11d24:	84ae                	mv	s1,a1
   11d26:	03800613          	li	a2,56
   11d2a:	4581                	li	a1,0
   11d2c:	0828                	addi	a0,sp,24
   11d2e:	f24a                	sd	s2,288(sp)
   11d30:	fe06                	sd	ra,312(sp)
   11d32:	fa22                	sd	s0,304(sp)
   11d34:	ee4e                	sd	s3,280(sp)
   11d36:	ea52                	sd	s4,272(sp)
   11d38:	e656                	sd	s5,264(sp)
   11d3a:	fdde                	sd	s7,248(sp)
   11d3c:	f9e2                	sd	s8,240(sp)
   11d3e:	f5e6                	sd	s9,232(sp)
   11d40:	f1ea                	sd	s10,224(sp)
   11d42:	edee                	sd	s11,216(sp)
   11d44:	e436                	sd	a3,8(sp)
   11d46:	bd3ff0ef          	jal	ra,11918 <memset>
   11d4a:	f601c783          	lbu	a5,-160(gp) # 18760 <__libc>
   11d4e:	02600513          	li	a0,38
   11d52:	28078a63          	beqz	a5,11fe6 <__pthread_create+0x2cc>
   11d56:	f6018913          	addi	s2,gp,-160 # 18760 <__libc>
   11d5a:	8a92                	mv	s5,tp
   11d5c:	00194783          	lbu	a5,1(s2)
   11d60:	f38a8a93          	addi	s5,s5,-200 # ffffffff7fffff38 <__global_pointer$+0xffffffff7ffe7738>
   11d64:	eba1                	bnez	a5,11db4 <__pthread_create+0x9a>
   11d66:	4bd000ef          	jal	ra,12a22 <__ofl_lock>
   11d6a:	6100                	ld	s0,0(a0)
   11d6c:	28041c63          	bnez	s0,12004 <__pthread_create+0x2ea>
   11d70:	4c9000ef          	jal	ra,12a38 <__ofl_unlock>
   11d74:	9501b503          	ld	a0,-1712(gp) # 18150 <__stderr_used>
   11d78:	c9fff0ef          	jal	ra,11a16 <init_file_lock>
   11d7c:	9281b503          	ld	a0,-1752(gp) # 18128 <__stdout_used>
   11d80:	c97ff0ef          	jal	ra,11a16 <init_file_lock>
   11d84:	9501b503          	ld	a0,-1712(gp) # 18150 <__stderr_used>
   11d88:	c8fff0ef          	jal	ra,11a16 <init_file_lock>
   11d8c:	478d                	li	a5,3
   11d8e:	1782                	slli	a5,a5,0x20
   11d90:	e8be                	sd	a5,80(sp)
   11d92:	08700893          	li	a7,135
   11d96:	4505                	li	a0,1
   11d98:	088c                	addi	a1,sp,80
   11d9a:	4601                	li	a2,0
   11d9c:	46a1                	li	a3,8
   11d9e:	00000073          	ecall
   11da2:	95818793          	addi	a5,gp,-1704 # 18158 <__pthread_tsd_main>
   11da6:	06fab823          	sd	a5,112(s5)
   11daa:	cabff0ef          	jal	ra,11a54 <__acquire_ptc>
   11dae:	4785                	li	a5,1
   11db0:	00f900a3          	sb	a5,1(s2)
   11db4:	c891                	beqz	s1,11dc8 <__pthread_create+0xae>
   11db6:	57fd                	li	a5,-1
   11db8:	00f48863          	beq	s1,a5,11dc8 <__pthread_create+0xae>
   11dbc:	03800613          	li	a2,56
   11dc0:	85a6                	mv	a1,s1
   11dc2:	0828                	addi	a0,sp,24
   11dc4:	4fd000ef          	jal	ra,12ac0 <memcpy>
   11dc8:	c8dff0ef          	jal	ra,11a54 <__acquire_ptc>
   11dcc:	c481                	beqz	s1,11dd4 <__pthread_create+0xba>
   11dce:	57fd                	li	a5,-1
   11dd0:	00f49863          	bne	s1,a5,11de0 <__pthread_create+0xc6>
   11dd4:	9341e783          	lwu	a5,-1740(gp) # 18134 <__default_stacksize>
   11dd8:	ec3e                	sd	a5,24(sp)
   11dda:	9301e783          	lwu	a5,-1744(gp) # 18130 <__default_guardsize>
   11dde:	f03e                	sd	a5,32(sp)
   11de0:	7722                	ld	a4,40(sp)
   11de2:	6a62                	ld	s4,24(sp)
   11de4:	01893783          	ld	a5,24(s2)
   11de8:	26070963          	beqz	a4,1205a <__pthread_create+0x340>
   11dec:	9481b603          	ld	a2,-1720(gp) # 18148 <__pthread_tsd_size>
   11df0:	ff077d93          	andi	s11,a4,-16
   11df4:	41470bb3          	sub	s7,a4,s4
   11df8:	963e                	add	a2,a2,a5
   11dfa:	003a5713          	srli	a4,s4,0x3
   11dfe:	20e67863          	bgeu	a2,a4,1200e <__pthread_create+0x2f4>
   11e02:	7ff00713          	li	a4,2047
   11e06:	20c76463          	bltu	a4,a2,1200e <__pthread_create+0x2f4>
   11e0a:	9481b703          	ld	a4,-1720(gp) # 18148 <__pthread_tsd_size>
   11e0e:	4581                	li	a1,0
   11e10:	40ed8c33          	sub	s8,s11,a4
   11e14:	40fc0db3          	sub	s11,s8,a5
   11e18:	856e                	mv	a0,s11
   11e1a:	affff0ef          	jal	ra,11918 <memset>
   11e1e:	200c0163          	beqz	s8,12020 <__pthread_create+0x306>
   11e22:	4c81                	li	s9,0
   11e24:	4981                	li	s3,0
   11e26:	01893503          	ld	a0,24(s2)
   11e2a:	417d8bb3          	sub	s7,s11,s7
   11e2e:	40ac0533          	sub	a0,s8,a0
   11e32:	558000ef          	jal	ra,1238a <__copy_tls>
   11e36:	f9818793          	addi	a5,gp,-104 # 18798 <__libc+0x38>
   11e3a:	ed5c                	sd	a5,152(a0)
   11e3c:	57c2                	lw	a5,48(sp)
   11e3e:	03353c23          	sd	s3,56(a0)
   11e42:	05453023          	sd	s4,64(a0)
   11e46:	05b53423          	sd	s11,72(a0)
   11e4a:	05753823          	sd	s7,80(a0)
   11e4e:	05953c23          	sd	s9,88(a0)
   11e52:	e108                	sd	a0,0(a0)
   11e54:	07853823          	sd	s8,112(a0)
   11e58:	842a                	mv	s0,a0
   11e5a:	26078c63          	beqz	a5,120d2 <__pthread_create+0x3b8>
   11e5e:	478d                	li	a5,3
   11e60:	d41c                	sw	a5,40(s0)
   11e62:	07840793          	addi	a5,s0,120
   11e66:	fc3c                	sd	a5,120(s0)
   11e68:	0b8ab783          	ld	a5,184(s5)
   11e6c:	5410                	lw	a2,40(s0)
   11e6e:	6bd9                	lui	s7,0x16
   11e70:	fc5c                	sd	a5,184(s0)
   11e72:	018ab783          	ld	a5,24(s5)
   11e76:	6555                	lui	a0,0x15
   11e78:	2601                	sext.w	a2,a2
   11e7a:	8d8b8593          	addi	a1,s7,-1832 # 158d8 <__func__.3458>
   11e7e:	ec1c                	sd	a5,24(s0)
   11e80:	7d050513          	addi	a0,a0,2000 # 157d0 <xdigits+0x10>
   11e84:	db6fe0ef          	jal	ra,1043a <printf>
   11e88:	6782                	ld	a5,0(sp)
   11e8a:	ff8dfd93          	andi	s11,s11,-8
   11e8e:	0888                	addi	a0,sp,80
   11e90:	fefdb023          	sd	a5,-32(s11)
   11e94:	67a2                	ld	a5,8(sp)
   11e96:	fe0d8d13          	addi	s10,s11,-32
   11e9a:	fefdb423          	sd	a5,-24(s11)
   11e9e:	57d2                	lw	a5,52(sp)
   11ea0:	00f037b3          	snez	a5,a5
   11ea4:	fefda823          	sw	a5,-16(s11)
   11ea8:	0c3000ef          	jal	ra,1276a <__block_app_sigs>
   11eac:	5410                	lw	a2,40(s0)
   11eae:	6555                	lui	a0,0x15
   11eb0:	8d8b8593          	addi	a1,s7,-1832
   11eb4:	2601                	sext.w	a2,a2
   11eb6:	7f850513          	addi	a0,a0,2040 # 157f8 <xdigits+0x38>
   11eba:	d80fe0ef          	jal	ra,1043a <printf>
   11ebe:	4621                	li	a2,8
   11ec0:	088c                	addi	a1,sp,80
   11ec2:	ff8d8513          	addi	a0,s11,-8
   11ec6:	3fb000ef          	jal	ra,12ac0 <memcpy>
   11eca:	ff8db703          	ld	a4,-8(s11)
   11ece:	57fd                	li	a5,-1
   11ed0:	1782                	slli	a5,a5,0x20
   11ed2:	17fd                	addi	a5,a5,-1
   11ed4:	8ff9                	and	a5,a5,a4
   11ed6:	fefdbc23          	sd	a5,-8(s11)
   11eda:	b7dff0ef          	jal	ra,11a56 <__tl_lock>
   11ede:	00492783          	lw	a5,4(s2)
   11ee2:	0017871b          	addiw	a4,a5,1
   11ee6:	00e92223          	sw	a4,4(s2)
   11eea:	e781                	bnez	a5,11ef2 <__pthread_create+0x1d8>
   11eec:	4785                	li	a5,1
   11eee:	00f901a3          	sb	a5,3(s2)
   11ef2:	5410                	lw	a2,40(s0)
   11ef4:	6559                	lui	a0,0x16
   11ef6:	8d8b8593          	addi	a1,s7,-1832
   11efa:	2601                	sext.w	a2,a2
   11efc:	82050513          	addi	a0,a0,-2016 # 15820 <xdigits+0x60>
   11f00:	d3afe0ef          	jal	ra,1043a <printf>
   11f04:	0c840793          	addi	a5,s0,200
   11f08:	6559                	lui	a0,0x16
   11f0a:	86be                	mv	a3,a5
   11f0c:	8622                	mv	a2,s0
   11f0e:	8d8b8593          	addi	a1,s7,-1832
   11f12:	84850513          	addi	a0,a0,-1976 # 15848 <xdigits+0x88>
   11f16:	e03e                	sd	a5,0(sp)
   11f18:	d22fe0ef          	jal	ra,1043a <printf>
   11f1c:	577d                	li	a4,-1
   11f1e:	6782                	ld	a5,0(sp)
   11f20:	1ae48b63          	beq	s1,a4,120d6 <__pthread_create+0x3bc>
   11f24:	6549                	lui	a0,0x12
   11f26:	c8c50513          	addi	a0,a0,-884 # 11c8c <start>
   11f2a:	007d1637          	lui	a2,0x7d1
   11f2e:	f0060613          	addi	a2,a2,-256 # 7d0f00 <__global_pointer$+0x7b8700>
   11f32:	85ea                	mv	a1,s10
   11f34:	fc818813          	addi	a6,gp,-56 # 187c8 <__thread_list_lock>
   11f38:	02040713          	addi	a4,s0,32
   11f3c:	86ea                	mv	a3,s10
   11f3e:	326000ef          	jal	ra,12264 <__clone>
   11f42:	5410                	lw	a2,40(s0)
   11f44:	84aa                	mv	s1,a0
   11f46:	6559                	lui	a0,0x16
   11f48:	2601                	sext.w	a2,a2
   11f4a:	8d8b8593          	addi	a1,s7,-1832
   11f4e:	87050513          	addi	a0,a0,-1936 # 15870 <xdigits+0xb0>
   11f52:	ce8fe0ef          	jal	ra,1043a <printf>
   11f56:	1804c463          	bltz	s1,120de <__pthread_create+0x3c4>
   11f5a:	57d2                	lw	a5,52(sp)
   11f5c:	cfa1                	beqz	a5,11fb4 <__pthread_create+0x29a>
   11f5e:	5008                	lw	a0,32(s0)
   11f60:	55e2                	lw	a1,56(sp)
   11f62:	07700893          	li	a7,119
   11f66:	1870                	addi	a2,sp,60
   11f68:	00000073          	ecall
   11f6c:	0005049b          	sext.w	s1,a0
   11f70:	468d                	li	a3,3
   11f72:	ff0d8513          	addi	a0,s11,-16
   11f76:	e091                	bnez	s1,11f7a <__pthread_create+0x260>
   11f78:	4681                	li	a3,0
   11f7a:	ff0da783          	lw	a5,-16(s11)
   11f7e:	2781                	sext.w	a5,a5
   11f80:	1605272f          	lr.w.aqrl	a4,(a0)
   11f84:	00f71563          	bne	a4,a5,11f8e <__pthread_create+0x274>
   11f88:	1ed5262f          	sc.w.aqrl	a2,a3,(a0)
   11f8c:	fa75                	bnez	a2,11f80 <__pthread_create+0x266>
   11f8e:	2701                	sext.w	a4,a4
   11f90:	fee795e3          	bne	a5,a4,11f7a <__pthread_create+0x260>
   11f94:	4709                	li	a4,2
   11f96:	00e79763          	bne	a5,a4,11fa4 <__pthread_create+0x28a>
   11f9a:	4585                	li	a1,1
   11f9c:	e02a                	sd	a0,0(sp)
   11f9e:	a8dff0ef          	jal	ra,11a2a <__wake.constprop.0>
   11fa2:	6502                	ld	a0,0(sp)
   11fa4:	c881                	beqz	s1,11fb4 <__pthread_create+0x29a>
   11fa6:	4681                	li	a3,0
   11fa8:	460d                	li	a2,3
   11faa:	4581                	li	a1,0
   11fac:	1f4010ef          	jal	ra,131a0 <__wait>
   11fb0:	1204c863          	bltz	s1,120e0 <__pthread_create+0x3c6>
   11fb4:	010ab783          	ld	a5,16(s5)
   11fb8:	01543423          	sd	s5,8(s0)
   11fbc:	e81c                	sd	a5,16(s0)
   11fbe:	e780                	sd	s0,8(a5)
   11fc0:	641c                	ld	a5,8(s0)
   11fc2:	eb80                	sd	s0,16(a5)
   11fc4:	aedff0ef          	jal	ra,11ab0 <__tl_unlock>
   11fc8:	0888                	addi	a0,sp,80
   11fca:	7b6000ef          	jal	ra,12780 <__restore_sigs>
   11fce:	a87ff0ef          	jal	ra,11a54 <__acquire_ptc>
   11fd2:	1204d263          	bgez	s1,120f6 <__pthread_create+0x3dc>
   11fd6:	00098663          	beqz	s3,11fe2 <__pthread_create+0x2c8>
   11fda:	85d2                	mv	a1,s4
   11fdc:	854e                	mv	a0,s3
   11fde:	73a000ef          	jal	ra,12718 <__munmap>
   11fe2:	4090053b          	negw	a0,s1
   11fe6:	70f2                	ld	ra,312(sp)
   11fe8:	7452                	ld	s0,304(sp)
   11fea:	74b2                	ld	s1,296(sp)
   11fec:	7912                	ld	s2,288(sp)
   11fee:	69f2                	ld	s3,280(sp)
   11ff0:	6a52                	ld	s4,272(sp)
   11ff2:	6ab2                	ld	s5,264(sp)
   11ff4:	6b12                	ld	s6,256(sp)
   11ff6:	7bee                	ld	s7,248(sp)
   11ff8:	7c4e                	ld	s8,240(sp)
   11ffa:	7cae                	ld	s9,232(sp)
   11ffc:	7d0e                	ld	s10,224(sp)
   11ffe:	6dee                	ld	s11,216(sp)
   12000:	6131                	addi	sp,sp,320
   12002:	8082                	ret
   12004:	8522                	mv	a0,s0
   12006:	a11ff0ef          	jal	ra,11a16 <init_file_lock>
   1200a:	7820                	ld	s0,112(s0)
   1200c:	b385                	j	11d6c <__pthread_create+0x52>
   1200e:	03093783          	ld	a5,48(s2)
   12012:	fff78a13          	addi	s4,a5,-1
   12016:	9a32                	add	s4,s4,a2
   12018:	40f007b3          	neg	a5,a5
   1201c:	00fa7a33          	and	s4,s4,a5
   12020:	4781                	li	a5,0
   12022:	577d                	li	a4,-1
   12024:	02200693          	li	a3,34
   12028:	460d                	li	a2,3
   1202a:	85d2                	mv	a1,s4
   1202c:	4501                	li	a0,0
   1202e:	63a000ef          	jal	ra,12668 <__mmap>
   12032:	57fd                	li	a5,-1
   12034:	89aa                	mv	s3,a0
   12036:	4c81                	li	s9,0
   12038:	08f50563          	beq	a0,a5,120c2 <__pthread_create+0x3a8>
   1203c:	9481b703          	ld	a4,-1720(gp) # 18148 <__pthread_tsd_size>
   12040:	40ea0733          	sub	a4,s4,a4
   12044:	00e98c33          	add	s8,s3,a4
   12048:	dc0d9fe3          	bnez	s11,11e26 <__pthread_create+0x10c>
   1204c:	01893d83          	ld	s11,24(s2)
   12050:	01998bb3          	add	s7,s3,s9
   12054:	41bc0db3          	sub	s11,s8,s11
   12058:	b3f9                	j	11e26 <__pthread_create+0x10c>
   1205a:	03093703          	ld	a4,48(s2)
   1205e:	7802                	ld	a6,32(sp)
   12060:	9a3e                	add	s4,s4,a5
   12062:	9481b603          	ld	a2,-1720(gp) # 18148 <__pthread_tsd_size>
   12066:	fffa0413          	addi	s0,s4,-1 # 3fffffff <__global_pointer$+0x3ffe77ff>
   1206a:	983a                	add	a6,a6,a4
   1206c:	943a                	add	s0,s0,a4
   1206e:	40e006b3          	neg	a3,a4
   12072:	187d                	addi	a6,a6,-1
   12074:	9432                	add	s0,s0,a2
   12076:	00d87cb3          	and	s9,a6,a3
   1207a:	8c75                	and	s0,s0,a3
   1207c:	01940a33          	add	s4,s0,s9
   12080:	040c8563          	beqz	s9,120ca <__pthread_create+0x3b0>
   12084:	4781                	li	a5,0
   12086:	577d                	li	a4,-1
   12088:	02200693          	li	a3,34
   1208c:	4601                	li	a2,0
   1208e:	85d2                	mv	a1,s4
   12090:	4501                	li	a0,0
   12092:	5d6000ef          	jal	ra,12668 <__mmap>
   12096:	57fd                	li	a5,-1
   12098:	89aa                	mv	s3,a0
   1209a:	02f50463          	beq	a0,a5,120c2 <__pthread_create+0x3a8>
   1209e:	460d                	li	a2,3
   120a0:	85a2                	mv	a1,s0
   120a2:	9566                	add	a0,a0,s9
   120a4:	642000ef          	jal	ra,126e6 <__mprotect>
   120a8:	c11d                	beqz	a0,120ce <__pthread_create+0x3b4>
   120aa:	4ae000ef          	jal	ra,12558 <__errno_location>
   120ae:	4118                	lw	a4,0(a0)
   120b0:	02600793          	li	a5,38
   120b4:	4d81                	li	s11,0
   120b6:	f8f703e3          	beq	a4,a5,1203c <__pthread_create+0x322>
   120ba:	85d2                	mv	a1,s4
   120bc:	854e                	mv	a0,s3
   120be:	65a000ef          	jal	ra,12718 <__munmap>
   120c2:	993ff0ef          	jal	ra,11a54 <__acquire_ptc>
   120c6:	452d                	li	a0,11
   120c8:	bf39                	j	11fe6 <__pthread_create+0x2cc>
   120ca:	4d81                	li	s11,0
   120cc:	bf91                	j	12020 <__pthread_create+0x306>
   120ce:	4d81                	li	s11,0
   120d0:	b7b5                	j	1203c <__pthread_create+0x322>
   120d2:	4789                	li	a5,2
   120d4:	b371                	j	11e60 <__pthread_create+0x146>
   120d6:	6549                	lui	a0,0x12
   120d8:	cf450513          	addi	a0,a0,-780 # 11cf4 <start_c11>
   120dc:	b5b9                	j	11f2a <__pthread_create+0x210>
   120de:	54d5                	li	s1,-11
   120e0:	00492783          	lw	a5,4(s2)
   120e4:	fff7871b          	addiw	a4,a5,-1
   120e8:	00e92223          	sw	a4,4(s2)
   120ec:	ec071ce3          	bnez	a4,11fc4 <__pthread_create+0x2aa>
   120f0:	000901a3          	sb	zero,3(s2)
   120f4:	bdc1                	j	11fc4 <__pthread_create+0x2aa>
   120f6:	5410                	lw	a2,40(s0)
   120f8:	6559                	lui	a0,0x16
   120fa:	8d8b8593          	addi	a1,s7,-1832
   120fe:	2601                	sext.w	a2,a2
   12100:	89850513          	addi	a0,a0,-1896 # 15898 <xdigits+0xd8>
   12104:	b36fe0ef          	jal	ra,1043a <printf>
   12108:	6559                	lui	a0,0x16
   1210a:	865a                	mv	a2,s6
   1210c:	8d8b8593          	addi	a1,s7,-1832
   12110:	8c050513          	addi	a0,a0,-1856 # 158c0 <xdigits+0x100>
   12114:	b26fe0ef          	jal	ra,1043a <printf>
   12118:	4501                	li	a0,0
   1211a:	008b3023          	sd	s0,0(s6)
   1211e:	b5e1                	j	11fe6 <__pthread_create+0x2cc>

0000000000012120 <dummy1>:
   12120:	8082                	ret

0000000000012122 <pthread_timedjoin_np>:
   12122:	7159                	addi	sp,sp,-112
   12124:	eca6                	sd	s1,88(sp)
   12126:	e4ce                	sd	s3,72(sp)
   12128:	84aa                	mv	s1,a0
   1212a:	69d9                	lui	s3,0x16
   1212c:	6559                	lui	a0,0x16
   1212e:	e0d2                	sd	s4,64(sp)
   12130:	8f050513          	addi	a0,a0,-1808 # 158f0 <__func__.3458+0x18>
   12134:	8a2e                	mv	s4,a1
   12136:	93898593          	addi	a1,s3,-1736 # 15938 <__func__.3016>
   1213a:	f486                	sd	ra,104(sp)
   1213c:	fc56                	sd	s5,56(sp)
   1213e:	f0a2                	sd	s0,96(sp)
   12140:	8ab2                	mv	s5,a2
   12142:	e8ca                	sd	s2,80(sp)
   12144:	f85a                	sd	s6,48(sp)
   12146:	f45e                	sd	s7,40(sp)
   12148:	f062                	sd	s8,32(sp)
   1214a:	ec66                	sd	s9,24(sp)
   1214c:	e86a                	sd	s10,16(sp)
   1214e:	aecfe0ef          	jal	ra,1043a <printf>
   12152:	100000ef          	jal	ra,12252 <__pthread_testcancel>
   12156:	006c                	addi	a1,sp,12
   12158:	4505                	li	a0,1
   1215a:	0ce000ef          	jal	ra,12228 <__pthread_setcancelstate>
   1215e:	47b2                	lw	a5,12(sp)
   12160:	e789                	bnez	a5,1216a <pthread_timedjoin_np+0x48>
   12162:	4581                	li	a1,0
   12164:	4501                	li	a0,0
   12166:	0c2000ef          	jal	ra,12228 <__pthread_setcancelstate>
   1216a:	4401                	li	s0,0
   1216c:	06e00b93          	li	s7,110
   12170:	4b59                	li	s6,22
   12172:	6c59                	lui	s8,0x16
   12174:	4c89                	li	s9,2
   12176:	02848d13          	addi	s10,s1,40
   1217a:	0284a903          	lw	s2,40(s1)
   1217e:	2901                	sext.w	s2,s2
   12180:	00090663          	beqz	s2,1218c <pthread_timedjoin_np+0x6a>
   12184:	01740463          	beq	s0,s7,1218c <pthread_timedjoin_np+0x6a>
   12188:	07641163          	bne	s0,s6,121ea <pthread_timedjoin_np+0xc8>
   1218c:	4532                	lw	a0,12(sp)
   1218e:	4581                	li	a1,0
   12190:	098000ef          	jal	ra,12228 <__pthread_setcancelstate>
   12194:	06e00793          	li	a5,110
   12198:	02f40a63          	beq	s0,a5,121cc <pthread_timedjoin_np+0xaa>
   1219c:	47d9                	li	a5,22
   1219e:	02f40763          	beq	s0,a5,121cc <pthread_timedjoin_np+0xaa>
   121a2:	8526                	mv	a0,s1
   121a4:	937ff0ef          	jal	ra,11ada <__tl_sync>
   121a8:	000a0563          	beqz	s4,121b2 <pthread_timedjoin_np+0x90>
   121ac:	70bc                	ld	a5,96(s1)
   121ae:	00fa3023          	sd	a5,0(s4)
   121b2:	7c88                	ld	a0,56(s1)
   121b4:	c501                	beqz	a0,121bc <pthread_timedjoin_np+0x9a>
   121b6:	60ac                	ld	a1,64(s1)
   121b8:	560000ef          	jal	ra,12718 <__munmap>
   121bc:	6559                	lui	a0,0x16
   121be:	93898593          	addi	a1,s3,-1736
   121c2:	92050513          	addi	a0,a0,-1760 # 15920 <__func__.3458+0x48>
   121c6:	a74fe0ef          	jal	ra,1043a <printf>
   121ca:	4401                	li	s0,0
   121cc:	70a6                	ld	ra,104(sp)
   121ce:	8522                	mv	a0,s0
   121d0:	7406                	ld	s0,96(sp)
   121d2:	64e6                	ld	s1,88(sp)
   121d4:	6946                	ld	s2,80(sp)
   121d6:	69a6                	ld	s3,72(sp)
   121d8:	6a06                	ld	s4,64(sp)
   121da:	7ae2                	ld	s5,56(sp)
   121dc:	7b42                	ld	s6,48(sp)
   121de:	7ba2                	ld	s7,40(sp)
   121e0:	7c02                	ld	s8,32(sp)
   121e2:	6ce2                	ld	s9,24(sp)
   121e4:	6d42                	ld	s10,16(sp)
   121e6:	6165                	addi	sp,sp,112
   121e8:	8082                	ret
   121ea:	864a                	mv	a2,s2
   121ec:	93898593          	addi	a1,s3,-1736
   121f0:	908c0513          	addi	a0,s8,-1784 # 15908 <__func__.3458+0x30>
   121f4:	a46fe0ef          	jal	ra,1043a <printf>
   121f8:	012cd563          	bge	s9,s2,12202 <pthread_timedjoin_np+0xe0>
   121fc:	00000023          	sb	zero,0(zero) # 0 <exit-0x10120>
   12200:	9002                	ebreak
   12202:	4705                	li	a4,1
   12204:	86d6                	mv	a3,s5
   12206:	4601                	li	a2,0
   12208:	85ca                	mv	a1,s2
   1220a:	856a                	mv	a0,s10
   1220c:	633000ef          	jal	ra,1303e <__timedwait_cp>
   12210:	842a                	mv	s0,a0
   12212:	b7a5                	j	1217a <pthread_timedjoin_np+0x58>

0000000000012214 <__pthread_join>:
   12214:	4601                	li	a2,0
   12216:	b731                	j	12122 <pthread_timedjoin_np>

0000000000012218 <pthread_tryjoin_np>:
   12218:	551c                	lw	a5,40(a0)
   1221a:	4709                	li	a4,2
   1221c:	2781                	sext.w	a5,a5
   1221e:	00e78363          	beq	a5,a4,12224 <pthread_tryjoin_np+0xc>
   12222:	bfcd                	j	12214 <__pthread_join>
   12224:	4541                	li	a0,16
   12226:	8082                	ret

0000000000012228 <__pthread_setcancelstate>:
   12228:	4789                	li	a5,2
   1222a:	02a7e163          	bltu	a5,a0,1224c <__pthread_setcancelstate+0x24>
   1222e:	8792                	mv	a5,tp
   12230:	f3878793          	addi	a5,a5,-200
   12234:	c591                	beqz	a1,12240 <__pthread_setcancelstate+0x18>
   12236:	0307c703          	lbu	a4,48(a5)
   1223a:	0ff77713          	zext.b	a4,a4
   1223e:	c198                	sw	a4,0(a1)
   12240:	0ff57513          	zext.b	a0,a0
   12244:	02a78823          	sb	a0,48(a5)
   12248:	4501                	li	a0,0
   1224a:	8082                	ret
   1224c:	4559                	li	a0,22
   1224e:	8082                	ret

0000000000012250 <__testcancel>:
   12250:	8082                	ret

0000000000012252 <__pthread_testcancel>:
   12252:	bffd                	j	12250 <__testcancel>

0000000000012254 <__unmapself>:
   12254:	0d700893          	li	a7,215
   12258:	00000073          	ecall
   1225c:	05d00893          	li	a7,93
   12260:	00000073          	ecall

0000000000012264 <__clone>:
   12264:	15c1                	addi	a1,a1,-16
   12266:	e188                	sd	a0,0(a1)
   12268:	e594                	sd	a3,8(a1)
   1226a:	8532                	mv	a0,a2
   1226c:	863a                	mv	a2,a4
   1226e:	86be                	mv	a3,a5
   12270:	8742                	mv	a4,a6
   12272:	0dc00893          	li	a7,220
   12276:	00000073          	ecall
   1227a:	c111                	beqz	a0,1227e <__clone+0x1a>
   1227c:	8082                	ret
   1227e:	6582                	ld	a1,0(sp)
   12280:	6522                	ld	a0,8(sp)
   12282:	9582                	jalr	a1
   12284:	05d00893          	li	a7,93
   12288:	00000073          	ecall

000000000001228c <__vm_wait>:
   1228c:	1101                	addi	sp,sp,-32
   1228e:	e822                	sd	s0,16(sp)
   12290:	96818413          	addi	s0,gp,-1688 # 18168 <vmlock>
   12294:	e426                	sd	s1,8(sp)
   12296:	ec06                	sd	ra,24(sp)
   12298:	00440493          	addi	s1,s0,4
   1229c:	4010                	lw	a2,0(s0)
   1229e:	2601                	sext.w	a2,a2
   122a0:	e611                	bnez	a2,122ac <__vm_wait+0x20>
   122a2:	60e2                	ld	ra,24(sp)
   122a4:	6442                	ld	s0,16(sp)
   122a6:	64a2                	ld	s1,8(sp)
   122a8:	6105                	addi	sp,sp,32
   122aa:	8082                	ret
   122ac:	4685                	li	a3,1
   122ae:	85a6                	mv	a1,s1
   122b0:	8522                	mv	a0,s0
   122b2:	6ef000ef          	jal	ra,131a0 <__wait>
   122b6:	b7dd                	j	1229c <__vm_wait+0x10>

00000000000122b8 <__vm_lock>:
   122b8:	96818793          	addi	a5,gp,-1688 # 18168 <vmlock>
   122bc:	4398                	lw	a4,0(a5)
   122be:	0007061b          	sext.w	a2,a4
   122c2:	2705                	addiw	a4,a4,1
   122c4:	1607a6af          	lr.w.aqrl	a3,(a5)
   122c8:	00c69563          	bne	a3,a2,122d2 <__vm_lock+0x1a>
   122cc:	1ee7a5af          	sc.w.aqrl	a1,a4,(a5)
   122d0:	f9f5                	bnez	a1,122c4 <__vm_lock+0xc>
   122d2:	0006871b          	sext.w	a4,a3
   122d6:	fee613e3          	bne	a2,a4,122bc <__vm_lock+0x4>
   122da:	8082                	ret

00000000000122dc <__vm_unlock>:
   122dc:	96818713          	addi	a4,gp,-1688 # 18168 <vmlock>
   122e0:	431c                	lw	a5,0(a4)
   122e2:	0007869b          	sext.w	a3,a5
   122e6:	37fd                	addiw	a5,a5,-1
   122e8:	1607262f          	lr.w.aqrl	a2,(a4)
   122ec:	00d61563          	bne	a2,a3,122f6 <__vm_unlock+0x1a>
   122f0:	1ef725af          	sc.w.aqrl	a1,a5,(a4)
   122f4:	f9f5                	bnez	a1,122e8 <__vm_unlock+0xc>
   122f6:	0006079b          	sext.w	a5,a2
   122fa:	fef693e3          	bne	a3,a5,122e0 <__vm_unlock+0x4>
   122fe:	4785                	li	a5,1
   12300:	02f69a63          	bne	a3,a5,12334 <__vm_unlock+0x58>
   12304:	435c                	lw	a5,4(a4)
   12306:	2781                	sext.w	a5,a5
   12308:	c795                	beqz	a5,12334 <__vm_unlock+0x58>
   1230a:	80000637          	lui	a2,0x80000
   1230e:	06200893          	li	a7,98
   12312:	96818513          	addi	a0,gp,-1688 # 18168 <vmlock>
   12316:	08100593          	li	a1,129
   1231a:	fff64613          	not	a2,a2
   1231e:	00000073          	ecall
   12322:	fda00793          	li	a5,-38
   12326:	00f51763          	bne	a0,a5,12334 <__vm_unlock+0x58>
   1232a:	96818513          	addi	a0,gp,-1688 # 18168 <vmlock>
   1232e:	4585                	li	a1,1
   12330:	00000073          	ecall
   12334:	8082                	ret

0000000000012336 <__init_tp>:
   12336:	1141                	addi	sp,sp,-16
   12338:	e022                	sd	s0,0(sp)
   1233a:	e406                	sd	ra,8(sp)
   1233c:	842a                	mv	s0,a0
   1233e:	e108                	sd	a0,0(a0)
   12340:	0c850513          	addi	a0,a0,200
   12344:	6ed000ef          	jal	ra,13230 <__set_thread_area>
   12348:	02054f63          	bltz	a0,12386 <__init_tp+0x50>
   1234c:	e501                	bnez	a0,12354 <__init_tp+0x1e>
   1234e:	4705                	li	a4,1
   12350:	f6e18023          	sb	a4,-160(gp) # 18760 <__libc>
   12354:	4789                	li	a5,2
   12356:	d41c                	sw	a5,40(s0)
   12358:	06000893          	li	a7,96
   1235c:	fc818513          	addi	a0,gp,-56 # 187c8 <__thread_list_lock>
   12360:	00000073          	ecall
   12364:	f9818793          	addi	a5,gp,-104 # 18798 <__libc+0x38>
   12368:	ec5c                	sd	a5,152(s0)
   1236a:	07840793          	addi	a5,s0,120
   1236e:	fc3c                	sd	a5,120(s0)
   12370:	f501b783          	ld	a5,-176(gp) # 18750 <__sysinfo>
   12374:	d008                	sw	a0,32(s0)
   12376:	e400                	sd	s0,8(s0)
   12378:	ec1c                	sd	a5,24(s0)
   1237a:	e800                	sd	s0,16(s0)
   1237c:	4501                	li	a0,0
   1237e:	60a2                	ld	ra,8(sp)
   12380:	6402                	ld	s0,0(sp)
   12382:	0141                	addi	sp,sp,16
   12384:	8082                	ret
   12386:	557d                	li	a0,-1
   12388:	bfdd                	j	1237e <__init_tp+0x48>

000000000001238a <__copy_tls>:
   1238a:	7139                	addi	sp,sp,-64
   1238c:	f6018713          	addi	a4,gp,-160 # 18760 <__libc>
   12390:	7714                	ld	a3,40(a4)
   12392:	f426                	sd	s1,40(sp)
   12394:	6f04                	ld	s1,24(a4)
   12396:	f822                	sd	s0,48(sp)
   12398:	7300                	ld	s0,32(a4)
   1239a:	068e                	slli	a3,a3,0x3
   1239c:	14e1                	addi	s1,s1,-8
   1239e:	8c95                	sub	s1,s1,a3
   123a0:	f3800693          	li	a3,-200
   123a4:	147d                	addi	s0,s0,-1
   123a6:	8e89                	sub	a3,a3,a0
   123a8:	f04a                	sd	s2,32(sp)
   123aa:	8c75                	and	s0,s0,a3
   123ac:	01073903          	ld	s2,16(a4)
   123b0:	e456                	sd	s5,8(sp)
   123b2:	94aa                	add	s1,s1,a0
   123b4:	942a                	add	s0,s0,a0
   123b6:	6a85                	lui	s5,0x1
   123b8:	ec4e                	sd	s3,24(sp)
   123ba:	e852                	sd	s4,16(sp)
   123bc:	e05a                	sd	s6,0(sp)
   123be:	fc06                	sd	ra,56(sp)
   123c0:	0c840b13          	addi	s6,s0,200
   123c4:	00848a13          	addi	s4,s1,8
   123c8:	f6018993          	addi	s3,gp,-160 # 18760 <__libc>
   123cc:	800a8a93          	addi	s5,s5,-2048 # 800 <exit-0xf920>
   123d0:	02091163          	bnez	s2,123f2 <__copy_tls+0x68>
   123d4:	0289b783          	ld	a5,40(s3)
   123d8:	70e2                	ld	ra,56(sp)
   123da:	8522                	mv	a0,s0
   123dc:	e09c                	sd	a5,0(s1)
   123de:	e064                	sd	s1,192(s0)
   123e0:	7442                	ld	s0,48(sp)
   123e2:	74a2                	ld	s1,40(sp)
   123e4:	7902                	ld	s2,32(sp)
   123e6:	69e2                	ld	s3,24(sp)
   123e8:	6a42                	ld	s4,16(sp)
   123ea:	6aa2                	ld	s5,8(sp)
   123ec:	6b02                	ld	s6,0(sp)
   123ee:	6121                	addi	sp,sp,64
   123f0:	8082                	ret
   123f2:	02893783          	ld	a5,40(s2)
   123f6:	00893583          	ld	a1,8(s2)
   123fa:	0a21                	addi	s4,s4,8
   123fc:	97da                	add	a5,a5,s6
   123fe:	97d6                	add	a5,a5,s5
   12400:	fefa3c23          	sd	a5,-8(s4)
   12404:	02893503          	ld	a0,40(s2)
   12408:	01093603          	ld	a2,16(s2)
   1240c:	955a                	add	a0,a0,s6
   1240e:	6b2000ef          	jal	ra,12ac0 <memcpy>
   12412:	00093903          	ld	s2,0(s2)
   12416:	bf6d                	j	123d0 <__copy_tls+0x46>

0000000000012418 <__init_tls>:
   12418:	01853e83          	ld	t4,24(a0)
   1241c:	02853883          	ld	a7,40(a0)
   12420:	9341a803          	lw	a6,-1740(gp) # 18134 <__default_stacksize>
   12424:	1101                	addi	sp,sp,-32
   12426:	00000637          	lui	a2,0x0
   1242a:	6474e337          	lui	t1,0x6474e
   1242e:	e426                	sd	s1,8(sp)
   12430:	e04a                	sd	s2,0(sp)
   12432:	ec06                	sd	ra,24(sp)
   12434:	e822                	sd	s0,16(sp)
   12436:	87f6                	mv	a5,t4
   12438:	4e01                	li	t3,0
   1243a:	4481                	li	s1,0
   1243c:	4901                	li	s2,0
   1243e:	4f19                	li	t5,6
   12440:	4f89                	li	t6,2
   12442:	00060613          	mv	a2,a2
   12446:	429d                	li	t0,7
   12448:	55130313          	addi	t1,t1,1361 # 6474e551 <__global_pointer$+0x64735d51>
   1244c:	008003b7          	lui	t2,0x800
   12450:	0a089663          	bnez	a7,124fc <__init_tls+0xe4>
   12454:	000e0463          	beqz	t3,1245c <__init_tls+0x44>
   12458:	9301aa23          	sw	a6,-1740(gp) # 18134 <__default_stacksize>
   1245c:	03000613          	li	a2,48
   12460:	4581                	li	a1,0
   12462:	f2018513          	addi	a0,gp,-224 # 18720 <main_tls>
   12466:	cb2ff0ef          	jal	ra,11918 <memset>
   1246a:	f2018793          	addi	a5,gp,-224 # 18720 <main_tls>
   1246e:	f6018693          	addi	a3,gp,-160 # 18760 <__libc>
   12472:	02090263          	beqz	s2,12496 <__init_tls+0x7e>
   12476:	01093703          	ld	a4,16(s2)
   1247a:	ea9c                	sd	a5,16(a3)
   1247c:	94ba                	add	s1,s1,a4
   1247e:	02093703          	ld	a4,32(s2)
   12482:	e784                	sd	s1,8(a5)
   12484:	eb98                	sd	a4,16(a5)
   12486:	02893703          	ld	a4,40(s2)
   1248a:	ef98                	sd	a4,24(a5)
   1248c:	03093703          	ld	a4,48(s2)
   12490:	f398                	sd	a4,32(a5)
   12492:	4705                	li	a4,1
   12494:	f698                	sd	a4,40(a3)
   12496:	0187b803          	ld	a6,24(a5)
   1249a:	678c                	ld	a1,8(a5)
   1249c:	7388                	ld	a0,32(a5)
   1249e:	00b80733          	add	a4,a6,a1
   124a2:	fff50613          	addi	a2,a0,-1
   124a6:	40e00733          	neg	a4,a4
   124aa:	8f71                	and	a4,a4,a2
   124ac:	9742                	add	a4,a4,a6
   124ae:	8e6d                	and	a2,a2,a1
   124b0:	ef98                	sd	a4,24(a5)
   124b2:	f790                	sd	a2,40(a5)
   124b4:	459d                	li	a1,7
   124b6:	00a5e463          	bltu	a1,a0,124be <__init_tls+0xa6>
   124ba:	45a1                	li	a1,8
   124bc:	f38c                	sd	a1,32(a5)
   124be:	739c                	ld	a5,32(a5)
   124c0:	0df78593          	addi	a1,a5,223
   124c4:	95b2                	add	a1,a1,a2
   124c6:	95ba                	add	a1,a1,a4
   124c8:	99e1                	andi	a1,a1,-8
   124ca:	f29c                	sd	a5,32(a3)
   124cc:	ee8c                	sd	a1,24(a3)
   124ce:	15000793          	li	a5,336
   124d2:	06b7fa63          	bgeu	a5,a1,12546 <__init_tls+0x12e>
   124d6:	0de00893          	li	a7,222
   124da:	4501                	li	a0,0
   124dc:	460d                	li	a2,3
   124de:	02200693          	li	a3,34
   124e2:	577d                	li	a4,-1
   124e4:	4781                	li	a5,0
   124e6:	00000073          	ecall
   124ea:	ea1ff0ef          	jal	ra,1238a <__copy_tls>
   124ee:	e49ff0ef          	jal	ra,12336 <__init_tp>
   124f2:	04055d63          	bgez	a0,1254c <__init_tls+0x134>
   124f6:	00000023          	sb	zero,0(zero) # 0 <exit-0x10120>
   124fa:	9002                	ebreak
   124fc:	438c                	lw	a1,0(a5)
   124fe:	01e59963          	bne	a1,t5,12510 <__init_tls+0xf8>
   12502:	6b84                	ld	s1,16(a5)
   12504:	409e84b3          	sub	s1,t4,s1
   12508:	7118                	ld	a4,32(a0)
   1250a:	18fd                	addi	a7,a7,-1
   1250c:	97ba                	add	a5,a5,a4
   1250e:	b789                	j	12450 <__init_tls+0x38>
   12510:	03f58463          	beq	a1,t6,12538 <__init_tls+0x120>
   12514:	02558763          	beq	a1,t0,12542 <__init_tls+0x12a>
   12518:	fe6598e3          	bne	a1,t1,12508 <__init_tls+0xf0>
   1251c:	7798                	ld	a4,40(a5)
   1251e:	02081593          	slli	a1,a6,0x20
   12522:	9181                	srli	a1,a1,0x20
   12524:	fee5f2e3          	bgeu	a1,a4,12508 <__init_tls+0xf0>
   12528:	00e3f463          	bgeu	t2,a4,12530 <__init_tls+0x118>
   1252c:	00800737          	lui	a4,0x800
   12530:	0007081b          	sext.w	a6,a4
   12534:	4e05                	li	t3,1
   12536:	bfc9                	j	12508 <__init_tls+0xf0>
   12538:	da61                	beqz	a2,12508 <__init_tls+0xf0>
   1253a:	6b84                	ld	s1,16(a5)
   1253c:	409604b3          	sub	s1,a2,s1
   12540:	b7e1                	j	12508 <__init_tls+0xf0>
   12542:	893e                	mv	s2,a5
   12544:	b7d1                	j	12508 <__init_tls+0xf0>
   12546:	dd018513          	addi	a0,gp,-560 # 185d0 <builtin_tls>
   1254a:	b745                	j	124ea <__init_tls+0xd2>
   1254c:	60e2                	ld	ra,24(sp)
   1254e:	6442                	ld	s0,16(sp)
   12550:	64a2                	ld	s1,8(sp)
   12552:	6902                	ld	s2,0(sp)
   12554:	6105                	addi	sp,sp,32
   12556:	8082                	ret

0000000000012558 <__errno_location>:
   12558:	8512                	mv	a0,tp
   1255a:	f5c50513          	addi	a0,a0,-164
   1255e:	8082                	ret

0000000000012560 <__strerror_l>:
   12560:	08300793          	li	a5,131
   12564:	00a7f363          	bgeu	a5,a0,1256a <__strerror_l+0xa>
   12568:	4501                	li	a0,0
   1256a:	67d9                	lui	a5,0x16
   1256c:	95078793          	addi	a5,a5,-1712 # 15950 <errmsgidx>
   12570:	0506                	slli	a0,a0,0x1
   12572:	953e                	add	a0,a0,a5
   12574:	00055503          	lhu	a0,0(a0)
   12578:	67d9                	lui	a5,0x16
   1257a:	758c                	ld	a1,40(a1)
   1257c:	a5878793          	addi	a5,a5,-1448 # 15a58 <errmsgstr>
   12580:	953e                	add	a0,a0,a5
   12582:	a839                	j	125a0 <__lctrans>

0000000000012584 <strerror>:
   12584:	8792                	mv	a5,tp
   12586:	fd07b583          	ld	a1,-48(a5)
   1258a:	bfd9                	j	12560 <__strerror_l>

000000000001258c <_Exit>:
   1258c:	87aa                	mv	a5,a0
   1258e:	05e00893          	li	a7,94
   12592:	00000073          	ecall
   12596:	05d00893          	li	a7,93
   1259a:	853e                	mv	a0,a5
   1259c:	bfdd                	j	12592 <_Exit+0x6>

000000000001259e <__lctrans_impl>:
   1259e:	8082                	ret

00000000000125a0 <__lctrans>:
   125a0:	bffd                	j	1259e <__lctrans_impl>

00000000000125a2 <__lctrans_cur>:
   125a2:	8792                	mv	a5,tp
   125a4:	fd07b783          	ld	a5,-48(a5)
   125a8:	778c                	ld	a1,40(a5)
   125aa:	bfd5                	j	1259e <__lctrans_impl>

00000000000125ac <__fpclassifyl>:
   125ac:	6721                	lui	a4,0x8
   125ae:	0305d693          	srli	a3,a1,0x30
   125b2:	177d                	addi	a4,a4,-1
   125b4:	05c2                	slli	a1,a1,0x10
   125b6:	8ef9                	and	a3,a3,a4
   125b8:	87aa                	mv	a5,a0
   125ba:	81c1                	srli	a1,a1,0x10
   125bc:	e691                	bnez	a3,125c8 <__fpclassifyl+0x1c>
   125be:	8dc9                	or	a1,a1,a0
   125c0:	00b03533          	snez	a0,a1
   125c4:	0509                	addi	a0,a0,2
   125c6:	8082                	ret
   125c8:	4511                	li	a0,4
   125ca:	00e69563          	bne	a3,a4,125d4 <__fpclassifyl+0x28>
   125ce:	8ddd                	or	a1,a1,a5
   125d0:	0015b513          	seqz	a0,a1
   125d4:	8082                	ret

00000000000125d6 <__signbitl>:
   125d6:	03f5d513          	srli	a0,a1,0x3f
   125da:	8082                	ret

00000000000125dc <frexpl>:
   125dc:	6721                	lui	a4,0x8
   125de:	1101                	addi	sp,sp,-32
   125e0:	0305d793          	srli	a5,a1,0x30
   125e4:	177d                	addi	a4,a4,-1
   125e6:	e822                	sd	s0,16(sp)
   125e8:	e426                	sd	s1,8(sp)
   125ea:	e04a                	sd	s2,0(sp)
   125ec:	ec06                	sd	ra,24(sp)
   125ee:	8932                	mv	s2,a2
   125f0:	00e7f633          	and	a2,a5,a4
   125f4:	84aa                	mv	s1,a0
   125f6:	842e                	mv	s0,a1
   125f8:	e239                	bnez	a2,1263e <frexpl+0x62>
   125fa:	4681                	li	a3,0
   125fc:	0e9010ef          	jal	ra,13ee4 <__eqtf2>
   12600:	cd05                	beqz	a0,12638 <frexpl+0x5c>
   12602:	67d9                	lui	a5,0x16
   12604:	1e87b683          	ld	a3,488(a5) # 161e8 <errmsgstr+0x790>
   12608:	4601                	li	a2,0
   1260a:	8526                	mv	a0,s1
   1260c:	85a2                	mv	a1,s0
   1260e:	169010ef          	jal	ra,13f76 <__multf3>
   12612:	864a                	mv	a2,s2
   12614:	fc9ff0ef          	jal	ra,125dc <frexpl>
   12618:	00092783          	lw	a5,0(s2)
   1261c:	84aa                	mv	s1,a0
   1261e:	842e                	mv	s0,a1
   12620:	f887879b          	addiw	a5,a5,-120
   12624:	00f92023          	sw	a5,0(s2)
   12628:	60e2                	ld	ra,24(sp)
   1262a:	85a2                	mv	a1,s0
   1262c:	6442                	ld	s0,16(sp)
   1262e:	6902                	ld	s2,0(sp)
   12630:	8526                	mv	a0,s1
   12632:	64a2                	ld	s1,8(sp)
   12634:	6105                	addi	sp,sp,32
   12636:	8082                	ret
   12638:	00092023          	sw	zero,0(s2)
   1263c:	b7f5                	j	12628 <frexpl+0x4c>
   1263e:	0006069b          	sext.w	a3,a2
   12642:	fee603e3          	beq	a2,a4,12628 <frexpl+0x4c>
   12646:	7771                	lui	a4,0xffffc
   12648:	2709                	addiw	a4,a4,2
   1264a:	9f35                	addw	a4,a4,a3
   1264c:	00e92023          	sw	a4,0(s2)
   12650:	7761                	lui	a4,0xffff8
   12652:	8ff9                	and	a5,a5,a4
   12654:	6711                	lui	a4,0x4
   12656:	1779                	addi	a4,a4,-2
   12658:	8fd9                	or	a5,a5,a4
   1265a:	01059413          	slli	s0,a1,0x10
   1265e:	17c2                	slli	a5,a5,0x30
   12660:	8041                	srli	s0,s0,0x10
   12662:	8c5d                	or	s0,s0,a5
   12664:	b7d1                	j	12628 <frexpl+0x4c>

0000000000012666 <dummy>:
   12666:	8082                	ret

0000000000012668 <__mmap>:
   12668:	7139                	addi	sp,sp,-64
   1266a:	f822                	sd	s0,48(sp)
   1266c:	fc06                	sd	ra,56(sp)
   1266e:	8436                	mv	s0,a3
   12670:	f426                	sd	s1,40(sp)
   12672:	03479693          	slli	a3,a5,0x34
   12676:	ca99                	beqz	a3,1268c <__mmap+0x24>
   12678:	ee1ff0ef          	jal	ra,12558 <__errno_location>
   1267c:	47d9                	li	a5,22
   1267e:	70e2                	ld	ra,56(sp)
   12680:	7442                	ld	s0,48(sp)
   12682:	c11c                	sw	a5,0(a0)
   12684:	74a2                	ld	s1,40(sp)
   12686:	557d                	li	a0,-1
   12688:	6121                	addi	sp,sp,64
   1268a:	8082                	ret
   1268c:	56f5                	li	a3,-3
   1268e:	8285                	srli	a3,a3,0x1
   12690:	00b6f663          	bgeu	a3,a1,1269c <__mmap+0x34>
   12694:	ec5ff0ef          	jal	ra,12558 <__errno_location>
   12698:	47b1                	li	a5,12
   1269a:	b7d5                	j	1267e <__mmap+0x16>
   1269c:	01047693          	andi	a3,s0,16
   126a0:	84aa                	mv	s1,a0
   126a2:	ca99                	beqz	a3,126b8 <__mmap+0x50>
   126a4:	ec3e                	sd	a5,24(sp)
   126a6:	e83a                	sd	a4,16(sp)
   126a8:	e432                	sd	a2,8(sp)
   126aa:	e02e                	sd	a1,0(sp)
   126ac:	be1ff0ef          	jal	ra,1228c <__vm_wait>
   126b0:	67e2                	ld	a5,24(sp)
   126b2:	6742                	ld	a4,16(sp)
   126b4:	6622                	ld	a2,8(sp)
   126b6:	6582                	ld	a1,0(sp)
   126b8:	0de00893          	li	a7,222
   126bc:	8526                	mv	a0,s1
   126be:	86a2                	mv	a3,s0
   126c0:	00000073          	ecall
   126c4:	57fd                	li	a5,-1
   126c6:	00f51a63          	bne	a0,a5,126da <__mmap+0x72>
   126ca:	e881                	bnez	s1,126da <__mmap+0x72>
   126cc:	03047413          	andi	s0,s0,48
   126d0:	02000793          	li	a5,32
   126d4:	00f41363          	bne	s0,a5,126da <__mmap+0x72>
   126d8:	5551                	li	a0,-12
   126da:	7442                	ld	s0,48(sp)
   126dc:	70e2                	ld	ra,56(sp)
   126de:	74a2                	ld	s1,40(sp)
   126e0:	6121                	addi	sp,sp,64
   126e2:	3a90006f          	j	1328a <__syscall_ret>

00000000000126e6 <__mprotect>:
   126e6:	f901b703          	ld	a4,-112(gp) # 18790 <__libc+0x30>
   126ea:	87aa                	mv	a5,a0
   126ec:	1141                	addi	sp,sp,-16
   126ee:	40e006b3          	neg	a3,a4
   126f2:	177d                	addi	a4,a4,-1
   126f4:	972e                	add	a4,a4,a1
   126f6:	97ba                	add	a5,a5,a4
   126f8:	8d75                	and	a0,a0,a3
   126fa:	00d7f5b3          	and	a1,a5,a3
   126fe:	e406                	sd	ra,8(sp)
   12700:	0e200893          	li	a7,226
   12704:	8d89                	sub	a1,a1,a0
   12706:	00000073          	ecall
   1270a:	381000ef          	jal	ra,1328a <__syscall_ret>
   1270e:	60a2                	ld	ra,8(sp)
   12710:	2501                	sext.w	a0,a0
   12712:	0141                	addi	sp,sp,16
   12714:	8082                	ret

0000000000012716 <dummy>:
   12716:	8082                	ret

0000000000012718 <__munmap>:
   12718:	1101                	addi	sp,sp,-32
   1271a:	ec06                	sd	ra,24(sp)
   1271c:	e42a                	sd	a0,8(sp)
   1271e:	e02e                	sd	a1,0(sp)
   12720:	b6dff0ef          	jal	ra,1228c <__vm_wait>
   12724:	6522                	ld	a0,8(sp)
   12726:	6582                	ld	a1,0(sp)
   12728:	0d700893          	li	a7,215
   1272c:	00000073          	ecall
   12730:	35b000ef          	jal	ra,1328a <__syscall_ret>
   12734:	60e2                	ld	ra,24(sp)
   12736:	2501                	sext.w	a0,a0
   12738:	6105                	addi	sp,sp,32
   1273a:	8082                	ret

000000000001273c <wctomb>:
   1273c:	c911                	beqz	a0,12750 <wctomb+0x14>
   1273e:	1141                	addi	sp,sp,-16
   12740:	4601                	li	a2,0
   12742:	e406                	sd	ra,8(sp)
   12744:	36b000ef          	jal	ra,132ae <wcrtomb>
   12748:	60a2                	ld	ra,8(sp)
   1274a:	2501                	sext.w	a0,a0
   1274c:	0141                	addi	sp,sp,16
   1274e:	8082                	ret
   12750:	4501                	li	a0,0
   12752:	8082                	ret

0000000000012754 <__block_all_sigs>:
   12754:	65d9                	lui	a1,0x16
   12756:	862a                	mv	a2,a0
   12758:	08700893          	li	a7,135
   1275c:	4501                	li	a0,0
   1275e:	39058593          	addi	a1,a1,912 # 16390 <all_mask>
   12762:	46a1                	li	a3,8
   12764:	00000073          	ecall
   12768:	8082                	ret

000000000001276a <__block_app_sigs>:
   1276a:	65d9                	lui	a1,0x16
   1276c:	862a                	mv	a2,a0
   1276e:	08700893          	li	a7,135
   12772:	4501                	li	a0,0
   12774:	39858593          	addi	a1,a1,920 # 16398 <app_mask>
   12778:	46a1                	li	a3,8
   1277a:	00000073          	ecall
   1277e:	8082                	ret

0000000000012780 <__restore_sigs>:
   12780:	85aa                	mv	a1,a0
   12782:	08700893          	li	a7,135
   12786:	4509                	li	a0,2
   12788:	4601                	li	a2,0
   1278a:	46a1                	li	a3,8
   1278c:	00000073          	ecall
   12790:	8082                	ret

0000000000012792 <__lockfile>:
   12792:	08c52783          	lw	a5,140(a0)
   12796:	2781                	sext.w	a5,a5
   12798:	8712                	mv	a4,tp
   1279a:	c00006b7          	lui	a3,0xc0000
   1279e:	f5872703          	lw	a4,-168(a4) # 3f58 <exit-0xc1c8>
   127a2:	16fd                	addi	a3,a3,-1
   127a4:	8ff5                	and	a5,a5,a3
   127a6:	08e78263          	beq	a5,a4,1282a <__lockfile+0x98>
   127aa:	08c50793          	addi	a5,a0,140
   127ae:	4601                	li	a2,0
   127b0:	1607a6af          	lr.w.aqrl	a3,(a5)
   127b4:	00c69563          	bne	a3,a2,127be <__lockfile+0x2c>
   127b8:	1ee7a5af          	sc.w.aqrl	a1,a4,(a5)
   127bc:	f9f5                	bnez	a1,127b0 <__lockfile+0x1e>
   127be:	2681                	sext.w	a3,a3
   127c0:	c295                	beqz	a3,127e4 <__lockfile+0x52>
   127c2:	40000837          	lui	a6,0x40000
   127c6:	01076733          	or	a4,a4,a6
   127ca:	4301                	li	t1,0
   127cc:	2701                	sext.w	a4,a4
   127ce:	fda00e13          	li	t3,-38
   127d2:	1607a6af          	lr.w.aqrl	a3,(a5)
   127d6:	00669563          	bne	a3,t1,127e0 <__lockfile+0x4e>
   127da:	1ee7a62f          	sc.w.aqrl	a2,a4,(a5)
   127de:	fa75                	bnez	a2,127d2 <__lockfile+0x40>
   127e0:	2681                	sext.w	a3,a3
   127e2:	e299                	bnez	a3,127e8 <__lockfile+0x56>
   127e4:	4505                	li	a0,1
   127e6:	8082                	ret
   127e8:	0106f5b3          	and	a1,a3,a6
   127ec:	0106e633          	or	a2,a3,a6
   127f0:	2581                	sext.w	a1,a1
   127f2:	2601                	sext.w	a2,a2
   127f4:	c185                	beqz	a1,12814 <__lockfile+0x82>
   127f6:	06200893          	li	a7,98
   127fa:	853e                	mv	a0,a5
   127fc:	08000593          	li	a1,128
   12800:	4681                	li	a3,0
   12802:	00000073          	ecall
   12806:	fdc516e3          	bne	a0,t3,127d2 <__lockfile+0x40>
   1280a:	853e                	mv	a0,a5
   1280c:	4581                	li	a1,0
   1280e:	00000073          	ecall
   12812:	b7c1                	j	127d2 <__lockfile+0x40>
   12814:	1607a5af          	lr.w.aqrl	a1,(a5)
   12818:	00d59563          	bne	a1,a3,12822 <__lockfile+0x90>
   1281c:	1ec7a52f          	sc.w.aqrl	a0,a2,(a5)
   12820:	f975                	bnez	a0,12814 <__lockfile+0x82>
   12822:	2581                	sext.w	a1,a1
   12824:	fad597e3          	bne	a1,a3,127d2 <__lockfile+0x40>
   12828:	b7f9                	j	127f6 <__lockfile+0x64>
   1282a:	4501                	li	a0,0
   1282c:	8082                	ret

000000000001282e <__unlockfile>:
   1282e:	08c50713          	addi	a4,a0,140
   12832:	4601                	li	a2,0
   12834:	08c52783          	lw	a5,140(a0)
   12838:	2781                	sext.w	a5,a5
   1283a:	160726af          	lr.w.aqrl	a3,(a4)
   1283e:	00f69563          	bne	a3,a5,12848 <__unlockfile+0x1a>
   12842:	1ec725af          	sc.w.aqrl	a1,a2,(a4)
   12846:	f9f5                	bnez	a1,1283a <__unlockfile+0xc>
   12848:	2681                	sext.w	a3,a3
   1284a:	fed795e3          	bne	a5,a3,12834 <__unlockfile+0x6>
   1284e:	02179693          	slli	a3,a5,0x21
   12852:	0206d263          	bgez	a3,12876 <__unlockfile+0x48>
   12856:	06200893          	li	a7,98
   1285a:	853a                	mv	a0,a4
   1285c:	08100593          	li	a1,129
   12860:	4605                	li	a2,1
   12862:	00000073          	ecall
   12866:	fda00793          	li	a5,-38
   1286a:	00f51663          	bne	a0,a5,12876 <__unlockfile+0x48>
   1286e:	853a                	mv	a0,a4
   12870:	4585                	li	a1,1
   12872:	00000073          	ecall
   12876:	8082                	ret

0000000000012878 <__aio_close>:
   12878:	8082                	ret

000000000001287a <__stdio_close>:
   1287a:	5d28                	lw	a0,120(a0)
   1287c:	1141                	addi	sp,sp,-16
   1287e:	e406                	sd	ra,8(sp)
   12880:	ff9ff0ef          	jal	ra,12878 <__aio_close>
   12884:	03900893          	li	a7,57
   12888:	00000073          	ecall
   1288c:	1ff000ef          	jal	ra,1328a <__syscall_ret>
   12890:	60a2                	ld	ra,8(sp)
   12892:	2501                	sext.w	a0,a0
   12894:	0141                	addi	sp,sp,16
   12896:	8082                	ret

0000000000012898 <__stdio_seek>:
   12898:	5d28                	lw	a0,120(a0)
   1289a:	1e70006f          	j	13280 <__lseek>

000000000001289e <__stdout_write>:
   1289e:	1101                	addi	sp,sp,-32
   128a0:	86b2                	mv	a3,a2
   128a2:	664d                	lui	a2,0x13
   128a4:	42860613          	addi	a2,a2,1064 # 13428 <__stdio_write>
   128a8:	ec06                	sd	ra,24(sp)
   128aa:	e530                	sd	a2,72(a0)
   128ac:	4110                	lw	a2,0(a0)
   128ae:	87aa                	mv	a5,a0
   128b0:	872e                	mv	a4,a1
   128b2:	04067613          	andi	a2,a2,64
   128b6:	ee09                	bnez	a2,128d0 <__stdout_write+0x32>
   128b8:	6595                	lui	a1,0x5
   128ba:	5d28                	lw	a0,120(a0)
   128bc:	48f5                	li	a7,29
   128be:	41358593          	addi	a1,a1,1043 # 5413 <exit-0xad0d>
   128c2:	0030                	addi	a2,sp,8
   128c4:	00000073          	ecall
   128c8:	c501                	beqz	a0,128d0 <__stdout_write+0x32>
   128ca:	567d                	li	a2,-1
   128cc:	08c7a823          	sw	a2,144(a5)
   128d0:	8636                	mv	a2,a3
   128d2:	85ba                	mv	a1,a4
   128d4:	853e                	mv	a0,a5
   128d6:	353000ef          	jal	ra,13428 <__stdio_write>
   128da:	60e2                	ld	ra,24(sp)
   128dc:	6105                	addi	sp,sp,32
   128de:	8082                	ret

00000000000128e0 <__towrite>:
   128e0:	08852783          	lw	a5,136(a0)
   128e4:	fff7871b          	addiw	a4,a5,-1
   128e8:	8fd9                	or	a5,a5,a4
   128ea:	08f52423          	sw	a5,136(a0)
   128ee:	411c                	lw	a5,0(a0)
   128f0:	0087f713          	andi	a4,a5,8
   128f4:	c711                	beqz	a4,12900 <__towrite+0x20>
   128f6:	0207e793          	ori	a5,a5,32
   128fa:	c11c                	sw	a5,0(a0)
   128fc:	557d                	li	a0,-1
   128fe:	8082                	ret
   12900:	6d3c                	ld	a5,88(a0)
   12902:	7138                	ld	a4,96(a0)
   12904:	00053823          	sd	zero,16(a0)
   12908:	fd1c                	sd	a5,56(a0)
   1290a:	f51c                	sd	a5,40(a0)
   1290c:	97ba                	add	a5,a5,a4
   1290e:	00053423          	sd	zero,8(a0)
   12912:	f11c                	sd	a5,32(a0)
   12914:	4501                	li	a0,0
   12916:	8082                	ret

0000000000012918 <__towrite_needs_stdio_exit>:
   12918:	2dd0006f          	j	133f4 <__stdio_exit>

000000000001291c <__fwritex>:
   1291c:	721c                	ld	a5,32(a2)
   1291e:	7179                	addi	sp,sp,-48
   12920:	f022                	sd	s0,32(sp)
   12922:	e84a                	sd	s2,16(sp)
   12924:	e44e                	sd	s3,8(sp)
   12926:	f406                	sd	ra,40(sp)
   12928:	ec26                	sd	s1,24(sp)
   1292a:	89aa                	mv	s3,a0
   1292c:	892e                	mv	s2,a1
   1292e:	8432                	mv	s0,a2
   12930:	c395                	beqz	a5,12954 <__fwritex+0x38>
   12932:	701c                	ld	a5,32(s0)
   12934:	7418                	ld	a4,40(s0)
   12936:	8f99                	sub	a5,a5,a4
   12938:	0327f463          	bgeu	a5,s2,12960 <__fwritex+0x44>
   1293c:	04843303          	ld	t1,72(s0)
   12940:	8522                	mv	a0,s0
   12942:	7402                	ld	s0,32(sp)
   12944:	70a2                	ld	ra,40(sp)
   12946:	64e2                	ld	s1,24(sp)
   12948:	864a                	mv	a2,s2
   1294a:	85ce                	mv	a1,s3
   1294c:	6942                	ld	s2,16(sp)
   1294e:	69a2                	ld	s3,8(sp)
   12950:	6145                	addi	sp,sp,48
   12952:	8302                	jr	t1
   12954:	8532                	mv	a0,a2
   12956:	f8bff0ef          	jal	ra,128e0 <__towrite>
   1295a:	dd61                	beqz	a0,12932 <__fwritex+0x16>
   1295c:	4501                	li	a0,0
   1295e:	a005                	j	1297e <__fwritex+0x62>
   12960:	09042783          	lw	a5,144(s0)
   12964:	84ca                	mv	s1,s2
   12966:	4729                	li	a4,10
   12968:	0207d363          	bgez	a5,1298e <__fwritex+0x72>
   1296c:	7408                	ld	a0,40(s0)
   1296e:	8626                	mv	a2,s1
   12970:	85ce                	mv	a1,s3
   12972:	14e000ef          	jal	ra,12ac0 <memcpy>
   12976:	7410                	ld	a2,40(s0)
   12978:	854a                	mv	a0,s2
   1297a:	94b2                	add	s1,s1,a2
   1297c:	f404                	sd	s1,40(s0)
   1297e:	70a2                	ld	ra,40(sp)
   12980:	7402                	ld	s0,32(sp)
   12982:	64e2                	ld	s1,24(sp)
   12984:	6942                	ld	s2,16(sp)
   12986:	69a2                	ld	s3,8(sp)
   12988:	6145                	addi	sp,sp,48
   1298a:	8082                	ret
   1298c:	84be                	mv	s1,a5
   1298e:	c485                	beqz	s1,129b6 <__fwritex+0x9a>
   12990:	fff48793          	addi	a5,s1,-1
   12994:	00f986b3          	add	a3,s3,a5
   12998:	0006c683          	lbu	a3,0(a3) # ffffffffc0000000 <__global_pointer$+0xffffffffbffe7800>
   1299c:	fee698e3          	bne	a3,a4,1298c <__fwritex+0x70>
   129a0:	643c                	ld	a5,72(s0)
   129a2:	8626                	mv	a2,s1
   129a4:	85ce                	mv	a1,s3
   129a6:	8522                	mv	a0,s0
   129a8:	9782                	jalr	a5
   129aa:	fc956ae3          	bltu	a0,s1,1297e <__fwritex+0x62>
   129ae:	99a6                	add	s3,s3,s1
   129b0:	409904b3          	sub	s1,s2,s1
   129b4:	bf65                	j	1296c <__fwritex+0x50>
   129b6:	84ca                	mv	s1,s2
   129b8:	bf55                	j	1296c <__fwritex+0x50>

00000000000129ba <fwrite>:
   129ba:	7139                	addi	sp,sp,-64
   129bc:	f822                	sd	s0,48(sp)
   129be:	f426                	sd	s1,40(sp)
   129c0:	f04a                	sd	s2,32(sp)
   129c2:	ec4e                	sd	s3,24(sp)
   129c4:	e456                	sd	s5,8(sp)
   129c6:	fc06                	sd	ra,56(sp)
   129c8:	e852                	sd	s4,16(sp)
   129ca:	02c58ab3          	mul	s5,a1,a2
   129ce:	89aa                	mv	s3,a0
   129d0:	892e                	mv	s2,a1
   129d2:	84b6                	mv	s1,a3
   129d4:	4401                	li	s0,0
   129d6:	c191                	beqz	a1,129da <fwrite+0x20>
   129d8:	8432                	mv	s0,a2
   129da:	08c4a783          	lw	a5,140(s1)
   129de:	4a01                	li	s4,0
   129e0:	02079713          	slli	a4,a5,0x20
   129e4:	00074663          	bltz	a4,129f0 <fwrite+0x36>
   129e8:	8526                	mv	a0,s1
   129ea:	da9ff0ef          	jal	ra,12792 <__lockfile>
   129ee:	8a2a                	mv	s4,a0
   129f0:	854e                	mv	a0,s3
   129f2:	8626                	mv	a2,s1
   129f4:	85d6                	mv	a1,s5
   129f6:	f27ff0ef          	jal	ra,1291c <__fwritex>
   129fa:	89aa                	mv	s3,a0
   129fc:	000a0563          	beqz	s4,12a06 <fwrite+0x4c>
   12a00:	8526                	mv	a0,s1
   12a02:	e2dff0ef          	jal	ra,1282e <__unlockfile>
   12a06:	013a8463          	beq	s5,s3,12a0e <fwrite+0x54>
   12a0a:	0329d433          	divu	s0,s3,s2
   12a0e:	70e2                	ld	ra,56(sp)
   12a10:	8522                	mv	a0,s0
   12a12:	7442                	ld	s0,48(sp)
   12a14:	74a2                	ld	s1,40(sp)
   12a16:	7902                	ld	s2,32(sp)
   12a18:	69e2                	ld	s3,24(sp)
   12a1a:	6a42                	ld	s4,16(sp)
   12a1c:	6aa2                	ld	s5,8(sp)
   12a1e:	6121                	addi	sp,sp,64
   12a20:	8082                	ret

0000000000012a22 <__ofl_lock>:
   12a22:	1141                	addi	sp,sp,-16
   12a24:	98018513          	addi	a0,gp,-1664 # 18180 <ofl_lock>
   12a28:	e406                	sd	ra,8(sp)
   12a2a:	4de000ef          	jal	ra,12f08 <__lock>
   12a2e:	60a2                	ld	ra,8(sp)
   12a30:	97818513          	addi	a0,gp,-1672 # 18178 <ofl_head>
   12a34:	0141                	addi	sp,sp,16
   12a36:	8082                	ret

0000000000012a38 <__ofl_unlock>:
   12a38:	98018513          	addi	a0,gp,-1664 # 18180 <ofl_lock>
   12a3c:	a355                	j	12fe0 <__unlock>

0000000000012a3e <memchr>:
   12a3e:	00757713          	andi	a4,a0,7
   12a42:	87aa                	mv	a5,a0
   12a44:	0ff5f593          	zext.b	a1,a1
   12a48:	cb19                	beqz	a4,12a5e <memchr+0x20>
   12a4a:	c635                	beqz	a2,12ab6 <memchr+0x78>
   12a4c:	0007c703          	lbu	a4,0(a5)
   12a50:	04b70863          	beq	a4,a1,12aa0 <memchr+0x62>
   12a54:	0785                	addi	a5,a5,1
   12a56:	0077f713          	andi	a4,a5,7
   12a5a:	167d                	addi	a2,a2,-1
   12a5c:	f77d                	bnez	a4,12a4a <memchr+0xc>
   12a5e:	4501                	li	a0,0
   12a60:	ce21                	beqz	a2,12ab8 <memchr+0x7a>
   12a62:	0007c703          	lbu	a4,0(a5)
   12a66:	02b70d63          	beq	a4,a1,12aa0 <memchr+0x62>
   12a6a:	9081b503          	ld	a0,-1784(gp) # 18108 <__SDATA_BEGIN__>
   12a6e:	471d                	li	a4,7
   12a70:	02a58533          	mul	a0,a1,a0
   12a74:	02c77663          	bgeu	a4,a2,12aa0 <memchr+0x62>
   12a78:	9101b883          	ld	a7,-1776(gp) # 18110 <__SDATA_BEGIN__+0x8>
   12a7c:	9181b803          	ld	a6,-1768(gp) # 18118 <__SDATA_BEGIN__+0x10>
   12a80:	431d                	li	t1,7
   12a82:	a029                	j	12a8c <memchr+0x4e>
   12a84:	1661                	addi	a2,a2,-8
   12a86:	07a1                	addi	a5,a5,8
   12a88:	02c37963          	bgeu	t1,a2,12aba <memchr+0x7c>
   12a8c:	6398                	ld	a4,0(a5)
   12a8e:	8f29                	xor	a4,a4,a0
   12a90:	011706b3          	add	a3,a4,a7
   12a94:	fff74713          	not	a4,a4
   12a98:	8f75                	and	a4,a4,a3
   12a9a:	01077733          	and	a4,a4,a6
   12a9e:	d37d                	beqz	a4,12a84 <memchr+0x46>
   12aa0:	853e                	mv	a0,a5
   12aa2:	97b2                	add	a5,a5,a2
   12aa4:	a021                	j	12aac <memchr+0x6e>
   12aa6:	0505                	addi	a0,a0,1
   12aa8:	00f50763          	beq	a0,a5,12ab6 <memchr+0x78>
   12aac:	00054703          	lbu	a4,0(a0)
   12ab0:	feb71be3          	bne	a4,a1,12aa6 <memchr+0x68>
   12ab4:	8082                	ret
   12ab6:	4501                	li	a0,0
   12ab8:	8082                	ret
   12aba:	4501                	li	a0,0
   12abc:	f275                	bnez	a2,12aa0 <memchr+0x62>
   12abe:	8082                	ret

0000000000012ac0 <memcpy>:
   12ac0:	0035f793          	andi	a5,a1,3
   12ac4:	3e078d63          	beqz	a5,12ebe <memcpy+0x3fe>
   12ac8:	22060063          	beqz	a2,12ce8 <memcpy+0x228>
   12acc:	87aa                	mv	a5,a0
   12ace:	a019                	j	12ad4 <memcpy+0x14>
   12ad0:	20060b63          	beqz	a2,12ce6 <memcpy+0x226>
   12ad4:	0005c683          	lbu	a3,0(a1)
   12ad8:	0585                	addi	a1,a1,1
   12ada:	0035f713          	andi	a4,a1,3
   12ade:	00d78023          	sb	a3,0(a5)
   12ae2:	167d                	addi	a2,a2,-1
   12ae4:	0785                	addi	a5,a5,1
   12ae6:	f76d                	bnez	a4,12ad0 <memcpy+0x10>
   12ae8:	0037f813          	andi	a6,a5,3
   12aec:	873e                	mv	a4,a5
   12aee:	1e080e63          	beqz	a6,12cea <memcpy+0x22a>
   12af2:	1141                	addi	sp,sp,-16
   12af4:	e422                	sd	s0,8(sp)
   12af6:	e026                	sd	s1,0(sp)
   12af8:	477d                	li	a4,31
   12afa:	0ac77863          	bgeu	a4,a2,12baa <memcpy+0xea>
   12afe:	4689                	li	a3,2
   12b00:	0005c883          	lbu	a7,0(a1)
   12b04:	4198                	lw	a4,0(a1)
   12b06:	30d80e63          	beq	a6,a3,12e22 <memcpy+0x362>
   12b0a:	468d                	li	a3,3
   12b0c:	28d80563          	beq	a6,a3,12d96 <memcpy+0x2d6>
   12b10:	0015c303          	lbu	t1,1(a1)
   12b14:	0025c803          	lbu	a6,2(a1)
   12b18:	fec60693          	addi	a3,a2,-20
   12b1c:	ff06f393          	andi	t2,a3,-16
   12b20:	00358413          	addi	s0,a1,3
   12b24:	00378493          	addi	s1,a5,3
   12b28:	03cd                	addi	t2,t2,19
   12b2a:	006780a3          	sb	t1,1(a5)
   12b2e:	01178023          	sb	a7,0(a5)
   12b32:	01078123          	sb	a6,2(a5)
   12b36:	93be                	add	t2,t2,a5
   12b38:	0046d593          	srli	a1,a3,0x4
   12b3c:	8322                	mv	t1,s0
   12b3e:	87a6                	mv	a5,s1
   12b40:	00132883          	lw	a7,1(t1)
   12b44:	00532803          	lw	a6,5(t1)
   12b48:	00932683          	lw	a3,9(t1)
   12b4c:	01875e1b          	srliw	t3,a4,0x18
   12b50:	00d32703          	lw	a4,13(t1)
   12b54:	0088929b          	slliw	t0,a7,0x8
   12b58:	00881f9b          	slliw	t6,a6,0x8
   12b5c:	00869f1b          	slliw	t5,a3,0x8
   12b60:	0188d89b          	srliw	a7,a7,0x18
   12b64:	0188581b          	srliw	a6,a6,0x18
   12b68:	0186d69b          	srliw	a3,a3,0x18
   12b6c:	00871e9b          	slliw	t4,a4,0x8
   12b70:	005e6e33          	or	t3,t3,t0
   12b74:	01f8e8b3          	or	a7,a7,t6
   12b78:	01e86833          	or	a6,a6,t5
   12b7c:	01d6e6b3          	or	a3,a3,t4
   12b80:	01c7a023          	sw	t3,0(a5)
   12b84:	0117a223          	sw	a7,4(a5)
   12b88:	0107a423          	sw	a6,8(a5)
   12b8c:	c7d4                	sw	a3,12(a5)
   12b8e:	07c1                	addi	a5,a5,16
   12b90:	0341                	addi	t1,t1,16
   12b92:	fa7797e3          	bne	a5,t2,12b40 <memcpy+0x80>
   12b96:	00158713          	addi	a4,a1,1
   12b9a:	0712                	slli	a4,a4,0x4
   12b9c:	0592                	slli	a1,a1,0x4
   12b9e:	1635                	addi	a2,a2,-19
   12ba0:	8e0d                	sub	a2,a2,a1
   12ba2:	00e487b3          	add	a5,s1,a4
   12ba6:	00e405b3          	add	a1,s0,a4
   12baa:	01067713          	andi	a4,a2,16
   12bae:	00867893          	andi	a7,a2,8
   12bb2:	00467813          	andi	a6,a2,4
   12bb6:	00267693          	andi	a3,a2,2
   12bba:	8a05                	andi	a2,a2,1
   12bbc:	34070263          	beqz	a4,12f00 <memcpy+0x440>
   12bc0:	0005c403          	lbu	s0,0(a1)
   12bc4:	00e5c703          	lbu	a4,14(a1)
   12bc8:	0085c383          	lbu	t2,8(a1)
   12bcc:	00878023          	sb	s0,0(a5)
   12bd0:	0015c403          	lbu	s0,1(a1)
   12bd4:	0095c283          	lbu	t0,9(a1)
   12bd8:	00a5cf83          	lbu	t6,10(a1)
   12bdc:	008780a3          	sb	s0,1(a5)
   12be0:	0025c403          	lbu	s0,2(a1)
   12be4:	00b5cf03          	lbu	t5,11(a1)
   12be8:	00c5ce83          	lbu	t4,12(a1)
   12bec:	00878123          	sb	s0,2(a5)
   12bf0:	0035c403          	lbu	s0,3(a1)
   12bf4:	00d5ce03          	lbu	t3,13(a1)
   12bf8:	00f5c303          	lbu	t1,15(a1)
   12bfc:	008781a3          	sb	s0,3(a5)
   12c00:	0045c403          	lbu	s0,4(a1)
   12c04:	00e78723          	sb	a4,14(a5)
   12c08:	00778423          	sb	t2,8(a5)
   12c0c:	00878223          	sb	s0,4(a5)
   12c10:	0055c403          	lbu	s0,5(a1)
   12c14:	005784a3          	sb	t0,9(a5)
   12c18:	01f78523          	sb	t6,10(a5)
   12c1c:	008782a3          	sb	s0,5(a5)
   12c20:	0065c403          	lbu	s0,6(a1)
   12c24:	01e785a3          	sb	t5,11(a5)
   12c28:	01d78623          	sb	t4,12(a5)
   12c2c:	00878323          	sb	s0,6(a5)
   12c30:	0075c403          	lbu	s0,7(a1)
   12c34:	01c786a3          	sb	t3,13(a5)
   12c38:	05c1                	addi	a1,a1,16
   12c3a:	008783a3          	sb	s0,7(a5)
   12c3e:	01078713          	addi	a4,a5,16
   12c42:	006787a3          	sb	t1,15(a5)
   12c46:	26088a63          	beqz	a7,12eba <memcpy+0x3fa>
   12c4a:	0065c783          	lbu	a5,6(a1)
   12c4e:	0005c283          	lbu	t0,0(a1)
   12c52:	0015cf83          	lbu	t6,1(a1)
   12c56:	0025cf03          	lbu	t5,2(a1)
   12c5a:	0035ce83          	lbu	t4,3(a1)
   12c5e:	0045ce03          	lbu	t3,4(a1)
   12c62:	0055c303          	lbu	t1,5(a1)
   12c66:	0075c883          	lbu	a7,7(a1)
   12c6a:	00f70323          	sb	a5,6(a4)
   12c6e:	00570023          	sb	t0,0(a4)
   12c72:	01f700a3          	sb	t6,1(a4)
   12c76:	01e70123          	sb	t5,2(a4)
   12c7a:	01d701a3          	sb	t4,3(a4)
   12c7e:	01c70223          	sb	t3,4(a4)
   12c82:	006702a3          	sb	t1,5(a4)
   12c86:	05a1                	addi	a1,a1,8
   12c88:	00870793          	addi	a5,a4,8
   12c8c:	011703a3          	sb	a7,7(a4)
   12c90:	26080663          	beqz	a6,12efc <memcpy+0x43c>
   12c94:	0025c703          	lbu	a4,2(a1)
   12c98:	0005c303          	lbu	t1,0(a1)
   12c9c:	0015c883          	lbu	a7,1(a1)
   12ca0:	0035c803          	lbu	a6,3(a1)
   12ca4:	00e78123          	sb	a4,2(a5)
   12ca8:	00678023          	sb	t1,0(a5)
   12cac:	011780a3          	sb	a7,1(a5)
   12cb0:	0591                	addi	a1,a1,4
   12cb2:	00478713          	addi	a4,a5,4
   12cb6:	010781a3          	sb	a6,3(a5)
   12cba:	1e068e63          	beqz	a3,12eb6 <memcpy+0x3f6>
   12cbe:	0005c783          	lbu	a5,0(a1)
   12cc2:	0015c683          	lbu	a3,1(a1)
   12cc6:	0589                	addi	a1,a1,2
   12cc8:	00f70023          	sb	a5,0(a4)
   12ccc:	00d700a3          	sb	a3,1(a4)
   12cd0:	00270793          	addi	a5,a4,2
   12cd4:	c609                	beqz	a2,12cde <memcpy+0x21e>
   12cd6:	0005c703          	lbu	a4,0(a1)
   12cda:	00e78023          	sb	a4,0(a5)
   12cde:	6422                	ld	s0,8(sp)
   12ce0:	6482                	ld	s1,0(sp)
   12ce2:	0141                	addi	sp,sp,16
   12ce4:	8082                	ret
   12ce6:	8082                	ret
   12ce8:	8082                	ret
   12cea:	46bd                	li	a3,15
   12cec:	04c6f963          	bgeu	a3,a2,12d3e <memcpy+0x27e>
   12cf0:	00b7e6b3          	or	a3,a5,a1
   12cf4:	8a9d                	andi	a3,a3,7
   12cf6:	ff060e93          	addi	t4,a2,-16
   12cfa:	1c069463          	bnez	a3,12ec2 <memcpy+0x402>
   12cfe:	020eb693          	sltiu	a3,t4,32
   12d02:	0016c693          	xori	a3,a3,1
   12d06:	0ff6f693          	zext.b	a3,a3
   12d0a:	1a068c63          	beqz	a3,12ec2 <memcpy+0x402>
   12d0e:	004ede93          	srli	t4,t4,0x4
   12d12:	001e8e13          	addi	t3,t4,1
   12d16:	86ae                	mv	a3,a1
   12d18:	0086b303          	ld	t1,8(a3)
   12d1c:	0006b883          	ld	a7,0(a3)
   12d20:	0805                	addi	a6,a6,1
   12d22:	00673423          	sd	t1,8(a4)
   12d26:	01173023          	sd	a7,0(a4)
   12d2a:	06c1                	addi	a3,a3,16
   12d2c:	0741                	addi	a4,a4,16
   12d2e:	ffc865e3          	bltu	a6,t3,12d18 <memcpy+0x258>
   12d32:	001e8713          	addi	a4,t4,1
   12d36:	0712                	slli	a4,a4,0x4
   12d38:	8a3d                	andi	a2,a2,15
   12d3a:	97ba                	add	a5,a5,a4
   12d3c:	95ba                	add	a1,a1,a4
   12d3e:	00867813          	andi	a6,a2,8
   12d42:	00467693          	andi	a3,a2,4
   12d46:	00267713          	andi	a4,a2,2
   12d4a:	8a05                	andi	a2,a2,1
   12d4c:	00080c63          	beqz	a6,12d64 <memcpy+0x2a4>
   12d50:	0005a883          	lw	a7,0(a1)
   12d54:	0045a803          	lw	a6,4(a1)
   12d58:	07a1                	addi	a5,a5,8
   12d5a:	05a1                	addi	a1,a1,8
   12d5c:	ff17ac23          	sw	a7,-8(a5)
   12d60:	ff07ae23          	sw	a6,-4(a5)
   12d64:	c691                	beqz	a3,12d70 <memcpy+0x2b0>
   12d66:	4194                	lw	a3,0(a1)
   12d68:	0791                	addi	a5,a5,4
   12d6a:	0591                	addi	a1,a1,4
   12d6c:	fed7ae23          	sw	a3,-4(a5)
   12d70:	18070a63          	beqz	a4,12f04 <memcpy+0x444>
   12d74:	0005c703          	lbu	a4,0(a1)
   12d78:	0015c683          	lbu	a3,1(a1)
   12d7c:	0589                	addi	a1,a1,2
   12d7e:	00e78023          	sb	a4,0(a5)
   12d82:	00d780a3          	sb	a3,1(a5)
   12d86:	00278713          	addi	a4,a5,2
   12d8a:	de39                	beqz	a2,12ce8 <memcpy+0x228>
   12d8c:	0005c783          	lbu	a5,0(a1)
   12d90:	00f70023          	sb	a5,0(a4)
   12d94:	8082                	ret
   12d96:	fec60693          	addi	a3,a2,-20
   12d9a:	ff06f393          	andi	t2,a3,-16
   12d9e:	00158413          	addi	s0,a1,1
   12da2:	00178493          	addi	s1,a5,1
   12da6:	03c5                	addi	t2,t2,17
   12da8:	01178023          	sb	a7,0(a5)
   12dac:	93be                	add	t2,t2,a5
   12dae:	0046d593          	srli	a1,a3,0x4
   12db2:	8322                	mv	t1,s0
   12db4:	87a6                	mv	a5,s1
   12db6:	00332883          	lw	a7,3(t1)
   12dba:	00732803          	lw	a6,7(t1)
   12dbe:	00b32683          	lw	a3,11(t1)
   12dc2:	00875e1b          	srliw	t3,a4,0x8
   12dc6:	00f32703          	lw	a4,15(t1)
   12dca:	0188929b          	slliw	t0,a7,0x18
   12dce:	01881f9b          	slliw	t6,a6,0x18
   12dd2:	01869f1b          	slliw	t5,a3,0x18
   12dd6:	0088d89b          	srliw	a7,a7,0x8
   12dda:	0088581b          	srliw	a6,a6,0x8
   12dde:	0086d69b          	srliw	a3,a3,0x8
   12de2:	01871e9b          	slliw	t4,a4,0x18
   12de6:	005e6e33          	or	t3,t3,t0
   12dea:	01f8e8b3          	or	a7,a7,t6
   12dee:	01e86833          	or	a6,a6,t5
   12df2:	01d6e6b3          	or	a3,a3,t4
   12df6:	01c7a023          	sw	t3,0(a5)
   12dfa:	0117a223          	sw	a7,4(a5)
   12dfe:	0107a423          	sw	a6,8(a5)
   12e02:	c7d4                	sw	a3,12(a5)
   12e04:	07c1                	addi	a5,a5,16
   12e06:	0341                	addi	t1,t1,16
   12e08:	fa7797e3          	bne	a5,t2,12db6 <memcpy+0x2f6>
   12e0c:	00158713          	addi	a4,a1,1
   12e10:	0712                	slli	a4,a4,0x4
   12e12:	0592                	slli	a1,a1,0x4
   12e14:	163d                	addi	a2,a2,-17
   12e16:	8e0d                	sub	a2,a2,a1
   12e18:	00e487b3          	add	a5,s1,a4
   12e1c:	00e405b3          	add	a1,s0,a4
   12e20:	b369                	j	12baa <memcpy+0xea>
   12e22:	0015c803          	lbu	a6,1(a1)
   12e26:	fec60693          	addi	a3,a2,-20
   12e2a:	ff06f393          	andi	t2,a3,-16
   12e2e:	00258413          	addi	s0,a1,2
   12e32:	00278493          	addi	s1,a5,2
   12e36:	03c9                	addi	t2,t2,18
   12e38:	01178023          	sb	a7,0(a5)
   12e3c:	010780a3          	sb	a6,1(a5)
   12e40:	93be                	add	t2,t2,a5
   12e42:	0046d593          	srli	a1,a3,0x4
   12e46:	8322                	mv	t1,s0
   12e48:	87a6                	mv	a5,s1
   12e4a:	00232883          	lw	a7,2(t1)
   12e4e:	00632803          	lw	a6,6(t1)
   12e52:	00a32683          	lw	a3,10(t1)
   12e56:	01075e1b          	srliw	t3,a4,0x10
   12e5a:	00e32703          	lw	a4,14(t1)
   12e5e:	0108929b          	slliw	t0,a7,0x10
   12e62:	01081f9b          	slliw	t6,a6,0x10
   12e66:	01069f1b          	slliw	t5,a3,0x10
   12e6a:	0108d89b          	srliw	a7,a7,0x10
   12e6e:	0108581b          	srliw	a6,a6,0x10
   12e72:	0106d69b          	srliw	a3,a3,0x10
   12e76:	01071e9b          	slliw	t4,a4,0x10
   12e7a:	005e6e33          	or	t3,t3,t0
   12e7e:	01f8e8b3          	or	a7,a7,t6
   12e82:	01e86833          	or	a6,a6,t5
   12e86:	01d6e6b3          	or	a3,a3,t4
   12e8a:	01c7a023          	sw	t3,0(a5)
   12e8e:	0117a223          	sw	a7,4(a5)
   12e92:	0107a423          	sw	a6,8(a5)
   12e96:	c7d4                	sw	a3,12(a5)
   12e98:	07c1                	addi	a5,a5,16
   12e9a:	0341                	addi	t1,t1,16
   12e9c:	fa7797e3          	bne	a5,t2,12e4a <memcpy+0x38a>
   12ea0:	00158713          	addi	a4,a1,1
   12ea4:	0712                	slli	a4,a4,0x4
   12ea6:	0592                	slli	a1,a1,0x4
   12ea8:	1639                	addi	a2,a2,-18
   12eaa:	8e0d                	sub	a2,a2,a1
   12eac:	00e487b3          	add	a5,s1,a4
   12eb0:	00e405b3          	add	a1,s0,a4
   12eb4:	b9dd                	j	12baa <memcpy+0xea>
   12eb6:	87ba                	mv	a5,a4
   12eb8:	bd31                	j	12cd4 <memcpy+0x214>
   12eba:	87ba                	mv	a5,a4
   12ebc:	bbd1                	j	12c90 <memcpy+0x1d0>
   12ebe:	87aa                	mv	a5,a0
   12ec0:	b125                	j	12ae8 <memcpy+0x28>
   12ec2:	004ede93          	srli	t4,t4,0x4
   12ec6:	001e8f13          	addi	t5,t4,1
   12eca:	0f12                	slli	t5,t5,0x4
   12ecc:	9f2e                	add	t5,t5,a1
   12ece:	872e                	mv	a4,a1
   12ed0:	86be                	mv	a3,a5
   12ed2:	00072e03          	lw	t3,0(a4)
   12ed6:	00472303          	lw	t1,4(a4)
   12eda:	00872883          	lw	a7,8(a4)
   12ede:	00c72803          	lw	a6,12(a4)
   12ee2:	01c6a023          	sw	t3,0(a3)
   12ee6:	0066a223          	sw	t1,4(a3)
   12eea:	0116a423          	sw	a7,8(a3)
   12eee:	0106a623          	sw	a6,12(a3)
   12ef2:	0741                	addi	a4,a4,16
   12ef4:	06c1                	addi	a3,a3,16
   12ef6:	fcef1ee3          	bne	t5,a4,12ed2 <memcpy+0x412>
   12efa:	bd25                	j	12d32 <memcpy+0x272>
   12efc:	873e                	mv	a4,a5
   12efe:	bb75                	j	12cba <memcpy+0x1fa>
   12f00:	873e                	mv	a4,a5
   12f02:	b391                	j	12c46 <memcpy+0x186>
   12f04:	873e                	mv	a4,a5
   12f06:	b551                	j	12d8a <memcpy+0x2ca>

0000000000012f08 <__lock>:
   12f08:	f6018693          	addi	a3,gp,-160 # 18760 <__libc>
   12f0c:	0036c603          	lbu	a2,3(a3)
   12f10:	872a                	mv	a4,a0
   12f12:	0186161b          	slliw	a2,a2,0x18
   12f16:	4186561b          	sraiw	a2,a2,0x18
   12f1a:	c271                	beqz	a2,12fde <__lock+0xd6>
   12f1c:	800005b7          	lui	a1,0x80000
   12f20:	4501                	li	a0,0
   12f22:	0585                	addi	a1,a1,1
   12f24:	160727af          	lr.w.aqrl	a5,(a4)
   12f28:	00a79663          	bne	a5,a0,12f34 <__lock+0x2c>
   12f2c:	1eb7282f          	sc.w.aqrl	a6,a1,(a4)
   12f30:	fe081ae3          	bnez	a6,12f24 <__lock+0x1c>
   12f34:	2781                	sext.w	a5,a5
   12f36:	00065463          	bgez	a2,12f3e <__lock+0x36>
   12f3a:	000681a3          	sb	zero,3(a3)
   12f3e:	c3c5                	beqz	a5,12fde <__lock+0xd6>
   12f40:	800006b7          	lui	a3,0x80000
   12f44:	45a9                	li	a1,10
   12f46:	fff6c813          	not	a6,a3
   12f4a:	0016861b          	addiw	a2,a3,1
   12f4e:	86be                	mv	a3,a5
   12f50:	0007d463          	bgez	a5,12f58 <__lock+0x50>
   12f54:	00f806bb          	addw	a3,a6,a5
   12f58:	00d6053b          	addw	a0,a2,a3
   12f5c:	160727af          	lr.w.aqrl	a5,(a4)
   12f60:	00d79663          	bne	a5,a3,12f6c <__lock+0x64>
   12f64:	1ea728af          	sc.w.aqrl	a7,a0,(a4)
   12f68:	fe089ae3          	bnez	a7,12f5c <__lock+0x54>
   12f6c:	2781                	sext.w	a5,a5
   12f6e:	06f68863          	beq	a3,a5,12fde <__lock+0xd6>
   12f72:	35fd                	addiw	a1,a1,-1
   12f74:	fde9                	bnez	a1,12f4e <__lock+0x46>
   12f76:	431c                	lw	a5,0(a4)
   12f78:	0007861b          	sext.w	a2,a5
   12f7c:	2785                	addiw	a5,a5,1
   12f7e:	160726af          	lr.w.aqrl	a3,(a4)
   12f82:	00c69563          	bne	a3,a2,12f8c <__lock+0x84>
   12f86:	1ef725af          	sc.w.aqrl	a1,a5,(a4)
   12f8a:	f9f5                	bnez	a1,12f7e <__lock+0x76>
   12f8c:	2681                	sext.w	a3,a3
   12f8e:	fed614e3          	bne	a2,a3,12f76 <__lock+0x6e>
   12f92:	80000837          	lui	a6,0x80000
   12f96:	fda00313          	li	t1,-38
   12f9a:	fff84e13          	not	t3,a6
   12f9e:	86be                	mv	a3,a5
   12fa0:	0207d363          	bgez	a5,12fc6 <__lock+0xbe>
   12fa4:	06200893          	li	a7,98
   12fa8:	853a                	mv	a0,a4
   12faa:	08000593          	li	a1,128
   12fae:	863e                	mv	a2,a5
   12fb0:	4681                	li	a3,0
   12fb2:	00000073          	ecall
   12fb6:	00651663          	bne	a0,t1,12fc2 <__lock+0xba>
   12fba:	853a                	mv	a0,a4
   12fbc:	4581                	li	a1,0
   12fbe:	00000073          	ecall
   12fc2:	00fe06bb          	addw	a3,t3,a5
   12fc6:	00d8063b          	addw	a2,a6,a3
   12fca:	160727af          	lr.w.aqrl	a5,(a4)
   12fce:	00d79563          	bne	a5,a3,12fd8 <__lock+0xd0>
   12fd2:	1ec725af          	sc.w.aqrl	a1,a2,(a4)
   12fd6:	f9f5                	bnez	a1,12fca <__lock+0xc2>
   12fd8:	2781                	sext.w	a5,a5
   12fda:	fcf692e3          	bne	a3,a5,12f9e <__lock+0x96>
   12fde:	8082                	ret

0000000000012fe0 <__unlock>:
   12fe0:	4118                	lw	a4,0(a0)
   12fe2:	87aa                	mv	a5,a0
   12fe4:	02071693          	slli	a3,a4,0x20
   12fe8:	0406da63          	bgez	a3,1303c <__unlock+0x5c>
   12fec:	800005b7          	lui	a1,0x80000
   12ff0:	fff5c593          	not	a1,a1
   12ff4:	4398                	lw	a4,0(a5)
   12ff6:	0007069b          	sext.w	a3,a4
   12ffa:	9f2d                	addw	a4,a4,a1
   12ffc:	1607a62f          	lr.w.aqrl	a2,(a5)
   13000:	00d61563          	bne	a2,a3,1300a <__unlock+0x2a>
   13004:	1ee7a52f          	sc.w.aqrl	a0,a4,(a5)
   13008:	f975                	bnez	a0,12ffc <__unlock+0x1c>
   1300a:	0006071b          	sext.w	a4,a2
   1300e:	fee693e3          	bne	a3,a4,12ff4 <__unlock+0x14>
   13012:	80000737          	lui	a4,0x80000
   13016:	0705                	addi	a4,a4,1
   13018:	02e68263          	beq	a3,a4,1303c <__unlock+0x5c>
   1301c:	06200893          	li	a7,98
   13020:	853e                	mv	a0,a5
   13022:	08100593          	li	a1,129
   13026:	4605                	li	a2,1
   13028:	00000073          	ecall
   1302c:	fda00713          	li	a4,-38
   13030:	00e51663          	bne	a0,a4,1303c <__unlock+0x5c>
   13034:	853e                	mv	a0,a5
   13036:	4585                	li	a1,1
   13038:	00000073          	ecall
   1303c:	8082                	ret

000000000001303e <__timedwait_cp>:
   1303e:	715d                	addi	sp,sp,-80
   13040:	fc26                	sd	s1,56(sp)
   13042:	f84a                	sd	s2,48(sp)
   13044:	84aa                	mv	s1,a0
   13046:	892e                	mv	s2,a1
   13048:	6559                	lui	a0,0x16
   1304a:	65d9                	lui	a1,0x16
   1304c:	f44e                	sd	s3,40(sp)
   1304e:	23058593          	addi	a1,a1,560 # 16230 <__func__.2721>
   13052:	89ba                	mv	s3,a4
   13054:	1f050513          	addi	a0,a0,496 # 161f0 <errmsgstr+0x798>
   13058:	e0a2                	sd	s0,64(sp)
   1305a:	ec56                	sd	s5,24(sp)
   1305c:	e486                	sd	ra,72(sp)
   1305e:	f052                	sd	s4,32(sp)
   13060:	8ab2                	mv	s5,a2
   13062:	8436                	mv	s0,a3
   13064:	bd6fd0ef          	jal	ra,1043a <printf>
   13068:	00098463          	beqz	s3,13070 <__timedwait_cp+0x32>
   1306c:	08000993          	li	s3,128
   13070:	cc29                	beqz	s0,130ca <__timedwait_cp+0x8c>
   13072:	6418                	ld	a4,8(s0)
   13074:	3b9ada37          	lui	s4,0x3b9ad
   13078:	9ffa0793          	addi	a5,s4,-1537 # 3b9ac9ff <__global_pointer$+0x3b9941ff>
   1307c:	00e7fc63          	bgeu	a5,a4,13094 <__timedwait_cp+0x56>
   13080:	4559                	li	a0,22
   13082:	60a6                	ld	ra,72(sp)
   13084:	6406                	ld	s0,64(sp)
   13086:	74e2                	ld	s1,56(sp)
   13088:	7942                	ld	s2,48(sp)
   1308a:	79a2                	ld	s3,40(sp)
   1308c:	7a02                	ld	s4,32(sp)
   1308e:	6ae2                	ld	s5,24(sp)
   13090:	6161                	addi	sp,sp,80
   13092:	8082                	ret
   13094:	858a                	mv	a1,sp
   13096:	8556                	mv	a0,s5
   13098:	19e000ef          	jal	ra,13236 <__clock_gettime>
   1309c:	f175                	bnez	a0,13080 <__timedwait_cp+0x42>
   1309e:	6782                	ld	a5,0(sp)
   130a0:	6014                	ld	a3,0(s0)
   130a2:	6722                	ld	a4,8(sp)
   130a4:	8e9d                	sub	a3,a3,a5
   130a6:	641c                	ld	a5,8(s0)
   130a8:	e036                	sd	a3,0(sp)
   130aa:	8f99                	sub	a5,a5,a4
   130ac:	e43e                	sd	a5,8(sp)
   130ae:	0007d863          	bgez	a5,130be <__timedwait_cp+0x80>
   130b2:	a00a0a13          	addi	s4,s4,-1536
   130b6:	16fd                	addi	a3,a3,-1
   130b8:	97d2                	add	a5,a5,s4
   130ba:	e036                	sd	a3,0(sp)
   130bc:	e43e                	sd	a5,8(sp)
   130be:	6782                	ld	a5,0(sp)
   130c0:	06e00513          	li	a0,110
   130c4:	fa07cfe3          	bltz	a5,13082 <__timedwait_cp+0x44>
   130c8:	840a                	mv	s0,sp
   130ca:	65d9                	lui	a1,0x16
   130cc:	6559                	lui	a0,0x16
   130ce:	06200613          	li	a2,98
   130d2:	22058593          	addi	a1,a1,544 # 16220 <__func__.2708>
   130d6:	20850513          	addi	a0,a0,520 # 16208 <errmsgstr+0x7b0>
   130da:	b60fd0ef          	jal	ra,1043a <printf>
   130de:	4781                	li	a5,0
   130e0:	8722                	mv	a4,s0
   130e2:	4801                	li	a6,0
   130e4:	86ca                	mv	a3,s2
   130e6:	864e                	mv	a2,s3
   130e8:	85a6                	mv	a1,s1
   130ea:	06200513          	li	a0,98
   130ee:	3f6000ef          	jal	ra,134e4 <__syscall_cp>
   130f2:	0005079b          	sext.w	a5,a0
   130f6:	fda00713          	li	a4,-38
   130fa:	00e79e63          	bne	a5,a4,13116 <__timedwait_cp+0xd8>
   130fe:	4781                	li	a5,0
   13100:	4801                	li	a6,0
   13102:	8722                	mv	a4,s0
   13104:	86ca                	mv	a3,s2
   13106:	4601                	li	a2,0
   13108:	85a6                	mv	a1,s1
   1310a:	06200513          	li	a0,98
   1310e:	3d6000ef          	jal	ra,134e4 <__syscall_cp>
   13112:	0005079b          	sext.w	a5,a0
   13116:	5771                	li	a4,-4
   13118:	00e78e63          	beq	a5,a4,13134 <__timedwait_cp+0xf6>
   1311c:	f9200713          	li	a4,-110
   13120:	00e78763          	beq	a5,a4,1312e <__timedwait_cp+0xf0>
   13124:	f8300713          	li	a4,-125
   13128:	4501                	li	a0,0
   1312a:	f4e79ce3          	bne	a5,a4,13082 <__timedwait_cp+0x44>
   1312e:	40f0053b          	negw	a0,a5
   13132:	bf81                	j	13082 <__timedwait_cp+0x44>
   13134:	9841a503          	lw	a0,-1660(gp) # 18184 <__eintr_valid_flag>
   13138:	2501                	sext.w	a0,a0
   1313a:	d521                	beqz	a0,13082 <__timedwait_cp+0x44>
   1313c:	4511                	li	a0,4
   1313e:	b791                	j	13082 <__timedwait_cp+0x44>

0000000000013140 <__timedwait>:
   13140:	715d                	addi	sp,sp,-80
   13142:	e0a2                	sd	s0,64(sp)
   13144:	fc26                	sd	s1,56(sp)
   13146:	842a                	mv	s0,a0
   13148:	84ae                	mv	s1,a1
   1314a:	4505                	li	a0,1
   1314c:	106c                	addi	a1,sp,44
   1314e:	e486                	sd	ra,72(sp)
   13150:	ec32                	sd	a2,24(sp)
   13152:	e836                	sd	a3,16(sp)
   13154:	e43a                	sd	a4,8(sp)
   13156:	8d2ff0ef          	jal	ra,12228 <__pthread_setcancelstate>
   1315a:	6722                	ld	a4,8(sp)
   1315c:	66c2                	ld	a3,16(sp)
   1315e:	6662                	ld	a2,24(sp)
   13160:	85a6                	mv	a1,s1
   13162:	8522                	mv	a0,s0
   13164:	edbff0ef          	jal	ra,1303e <__timedwait_cp>
   13168:	842a                	mv	s0,a0
   1316a:	5532                	lw	a0,44(sp)
   1316c:	4581                	li	a1,0
   1316e:	8baff0ef          	jal	ra,12228 <__pthread_setcancelstate>
   13172:	60a6                	ld	ra,72(sp)
   13174:	8522                	mv	a0,s0
   13176:	6406                	ld	s0,64(sp)
   13178:	74e2                	ld	s1,56(sp)
   1317a:	6161                	addi	sp,sp,80
   1317c:	8082                	ret

000000000001317e <a_fetch_add>:
   1317e:	87aa                	mv	a5,a0
   13180:	4398                	lw	a4,0(a5)
   13182:	0007051b          	sext.w	a0,a4
   13186:	9f2d                	addw	a4,a4,a1
   13188:	1607a6af          	lr.w.aqrl	a3,(a5)
   1318c:	00a69563          	bne	a3,a0,13196 <a_fetch_add+0x18>
   13190:	1ee7a62f          	sc.w.aqrl	a2,a4,(a5)
   13194:	fa75                	bnez	a2,13188 <a_fetch_add+0xa>
   13196:	0006871b          	sext.w	a4,a3
   1319a:	fee513e3          	bne	a0,a4,13180 <a_fetch_add+0x2>
   1319e:	8082                	ret

00000000000131a0 <__wait>:
   131a0:	7179                	addi	sp,sp,-48
   131a2:	f022                	sd	s0,32(sp)
   131a4:	ec26                	sd	s1,24(sp)
   131a6:	e84a                	sd	s2,16(sp)
   131a8:	e44e                	sd	s3,8(sp)
   131aa:	f406                	sd	ra,40(sp)
   131ac:	84aa                	mv	s1,a0
   131ae:	842e                	mv	s0,a1
   131b0:	8932                	mv	s2,a2
   131b2:	89b6                	mv	s3,a3
   131b4:	c299                	beqz	a3,131ba <__wait+0x1a>
   131b6:	08000993          	li	s3,128
   131ba:	06500713          	li	a4,101
   131be:	a039                	j	131cc <__wait+0x2c>
   131c0:	409c                	lw	a5,0(s1)
   131c2:	2781                	sext.w	a5,a5
   131c4:	05279f63          	bne	a5,s2,13222 <__wait+0x82>
   131c8:	0330000f          	fence	rw,rw
   131cc:	377d                	addiw	a4,a4,-1
   131ce:	cb11                	beqz	a4,131e2 <__wait+0x42>
   131d0:	d865                	beqz	s0,131c0 <__wait+0x20>
   131d2:	401c                	lw	a5,0(s0)
   131d4:	2781                	sext.w	a5,a5
   131d6:	d7ed                	beqz	a5,131c0 <__wait+0x20>
   131d8:	4585                	li	a1,1
   131da:	8522                	mv	a0,s0
   131dc:	fa3ff0ef          	jal	ra,1317e <a_fetch_add>
   131e0:	a011                	j	131e4 <__wait+0x44>
   131e2:	f87d                	bnez	s0,131d8 <__wait+0x38>
   131e4:	fda00713          	li	a4,-38
   131e8:	409c                	lw	a5,0(s1)
   131ea:	2781                	sext.w	a5,a5
   131ec:	01278c63          	beq	a5,s2,13204 <__wait+0x64>
   131f0:	c80d                	beqz	s0,13222 <__wait+0x82>
   131f2:	8522                	mv	a0,s0
   131f4:	7402                	ld	s0,32(sp)
   131f6:	70a2                	ld	ra,40(sp)
   131f8:	64e2                	ld	s1,24(sp)
   131fa:	6942                	ld	s2,16(sp)
   131fc:	69a2                	ld	s3,8(sp)
   131fe:	55fd                	li	a1,-1
   13200:	6145                	addi	sp,sp,48
   13202:	bfb5                	j	1317e <a_fetch_add>
   13204:	06200893          	li	a7,98
   13208:	8526                	mv	a0,s1
   1320a:	85ce                	mv	a1,s3
   1320c:	864a                	mv	a2,s2
   1320e:	4681                	li	a3,0
   13210:	00000073          	ecall
   13214:	fce51ae3          	bne	a0,a4,131e8 <__wait+0x48>
   13218:	8526                	mv	a0,s1
   1321a:	4581                	li	a1,0
   1321c:	00000073          	ecall
   13220:	b7e1                	j	131e8 <__wait+0x48>
   13222:	70a2                	ld	ra,40(sp)
   13224:	7402                	ld	s0,32(sp)
   13226:	64e2                	ld	s1,24(sp)
   13228:	6942                	ld	s2,16(sp)
   1322a:	69a2                	ld	s3,8(sp)
   1322c:	6145                	addi	sp,sp,48
   1322e:	8082                	ret

0000000000013230 <__set_thread_area>:
   13230:	822a                	mv	tp,a0
   13232:	4501                	li	a0,0
   13234:	8082                	ret

0000000000013236 <__clock_gettime>:
   13236:	87aa                	mv	a5,a0
   13238:	07100893          	li	a7,113
   1323c:	00000073          	ecall
   13240:	fda00693          	li	a3,-38
   13244:	2501                	sext.w	a0,a0
   13246:	02d51363          	bne	a0,a3,1326c <__clock_gettime+0x36>
   1324a:	5529                	li	a0,-22
   1324c:	e385                	bnez	a5,1326c <__clock_gettime+0x36>
   1324e:	872e                	mv	a4,a1
   13250:	852e                	mv	a0,a1
   13252:	0a900893          	li	a7,169
   13256:	4581                	li	a1,0
   13258:	00000073          	ecall
   1325c:	6710                	ld	a2,8(a4)
   1325e:	3e800693          	li	a3,1000
   13262:	853e                	mv	a0,a5
   13264:	02c686bb          	mulw	a3,a3,a2
   13268:	e714                	sd	a3,8(a4)
   1326a:	8082                	ret
   1326c:	1141                	addi	sp,sp,-16
   1326e:	e406                	sd	ra,8(sp)
   13270:	01a000ef          	jal	ra,1328a <__syscall_ret>
   13274:	60a2                	ld	ra,8(sp)
   13276:	0005079b          	sext.w	a5,a0
   1327a:	853e                	mv	a0,a5
   1327c:	0141                	addi	sp,sp,16
   1327e:	8082                	ret

0000000000013280 <__lseek>:
   13280:	03e00893          	li	a7,62
   13284:	00000073          	ecall
   13288:	a009                	j	1328a <__syscall_ret>

000000000001328a <__syscall_ret>:
   1328a:	1141                	addi	sp,sp,-16
   1328c:	e022                	sd	s0,0(sp)
   1328e:	e406                	sd	ra,8(sp)
   13290:	77fd                	lui	a5,0xfffff
   13292:	842a                	mv	s0,a0
   13294:	00a7e663          	bltu	a5,a0,132a0 <__syscall_ret+0x16>
   13298:	60a2                	ld	ra,8(sp)
   1329a:	6402                	ld	s0,0(sp)
   1329c:	0141                	addi	sp,sp,16
   1329e:	8082                	ret
   132a0:	ab8ff0ef          	jal	ra,12558 <__errno_location>
   132a4:	4080043b          	negw	s0,s0
   132a8:	c100                	sw	s0,0(a0)
   132aa:	557d                	li	a0,-1
   132ac:	b7f5                	j	13298 <__syscall_ret+0xe>

00000000000132ae <wcrtomb>:
   132ae:	c909                	beqz	a0,132c0 <wcrtomb+0x12>
   132b0:	07f00693          	li	a3,127
   132b4:	0005879b          	sext.w	a5,a1
   132b8:	00b6ec63          	bltu	a3,a1,132d0 <wcrtomb+0x22>
   132bc:	00b50023          	sb	a1,0(a0)
   132c0:	4505                	li	a0,1
   132c2:	8082                	ret
   132c4:	00b50023          	sb	a1,0(a0)
   132c8:	4505                	li	a0,1
   132ca:	60a2                	ld	ra,8(sp)
   132cc:	0141                	addi	sp,sp,16
   132ce:	8082                	ret
   132d0:	1141                	addi	sp,sp,-16
   132d2:	e406                	sd	ra,8(sp)
   132d4:	8712                	mv	a4,tp
   132d6:	fd073703          	ld	a4,-48(a4) # ffffffff7fffffd0 <__global_pointer$+0xffffffff7ffe77d0>
   132da:	6318                	ld	a4,0(a4)
   132dc:	ef11                	bnez	a4,132f8 <wcrtomb+0x4a>
   132de:	7749                	lui	a4,0xffff2
   132e0:	0807071b          	addiw	a4,a4,128
   132e4:	9fb9                	addw	a5,a5,a4
   132e6:	fcf6ffe3          	bgeu	a3,a5,132c4 <wcrtomb+0x16>
   132ea:	a6eff0ef          	jal	ra,12558 <__errno_location>
   132ee:	05400793          	li	a5,84
   132f2:	c11c                	sw	a5,0(a0)
   132f4:	557d                	li	a0,-1
   132f6:	bfd1                	j	132ca <wcrtomb+0x1c>
   132f8:	7ff00713          	li	a4,2047
   132fc:	02f76063          	bltu	a4,a5,1331c <wcrtomb+0x6e>
   13300:	4065d79b          	sraiw	a5,a1,0x6
   13304:	03f5f593          	andi	a1,a1,63
   13308:	fc07e793          	ori	a5,a5,-64
   1330c:	f805e593          	ori	a1,a1,-128
   13310:	00f50023          	sb	a5,0(a0)
   13314:	00b500a3          	sb	a1,1(a0)
   13318:	4509                	li	a0,2
   1331a:	bf45                	j	132ca <wcrtomb+0x1c>
   1331c:	6735                	lui	a4,0xd
   1331e:	7ff70713          	addi	a4,a4,2047 # d7ff <exit-0x2921>
   13322:	00f77763          	bgeu	a4,a5,13330 <wcrtomb+0x82>
   13326:	7749                	lui	a4,0xffff2
   13328:	9f3d                	addw	a4,a4,a5
   1332a:	6689                	lui	a3,0x2
   1332c:	02d77863          	bgeu	a4,a3,1335c <wcrtomb+0xae>
   13330:	40c5d79b          	sraiw	a5,a1,0xc
   13334:	fe07e793          	ori	a5,a5,-32
   13338:	00f50023          	sb	a5,0(a0)
   1333c:	4065d79b          	sraiw	a5,a1,0x6
   13340:	03f7f793          	andi	a5,a5,63
   13344:	03f5f593          	andi	a1,a1,63
   13348:	f807e793          	ori	a5,a5,-128
   1334c:	f805e593          	ori	a1,a1,-128
   13350:	00f500a3          	sb	a5,1(a0)
   13354:	00b50123          	sb	a1,2(a0)
   13358:	450d                	li	a0,3
   1335a:	bf85                	j	132ca <wcrtomb+0x1c>
   1335c:	7741                	lui	a4,0xffff0
   1335e:	9fb9                	addw	a5,a5,a4
   13360:	00100737          	lui	a4,0x100
   13364:	f8e7f3e3          	bgeu	a5,a4,132ea <wcrtomb+0x3c>
   13368:	4125d79b          	sraiw	a5,a1,0x12
   1336c:	ff07e793          	ori	a5,a5,-16
   13370:	00f50023          	sb	a5,0(a0)
   13374:	40c5d79b          	sraiw	a5,a1,0xc
   13378:	03f7f793          	andi	a5,a5,63
   1337c:	f807e793          	ori	a5,a5,-128
   13380:	00f500a3          	sb	a5,1(a0)
   13384:	4065d79b          	sraiw	a5,a1,0x6
   13388:	03f7f793          	andi	a5,a5,63
   1338c:	03f5f593          	andi	a1,a1,63
   13390:	f807e793          	ori	a5,a5,-128
   13394:	f805e593          	ori	a1,a1,-128
   13398:	00f50123          	sb	a5,2(a0)
   1339c:	00b501a3          	sb	a1,3(a0)
   133a0:	4511                	li	a0,4
   133a2:	b725                	j	132ca <wcrtomb+0x1c>

00000000000133a4 <close_file>:
   133a4:	c539                	beqz	a0,133f2 <close_file+0x4e>
   133a6:	08c52783          	lw	a5,140(a0)
   133aa:	1141                	addi	sp,sp,-16
   133ac:	e022                	sd	s0,0(sp)
   133ae:	e406                	sd	ra,8(sp)
   133b0:	02079713          	slli	a4,a5,0x20
   133b4:	842a                	mv	s0,a0
   133b6:	00074463          	bltz	a4,133be <close_file+0x1a>
   133ba:	bd8ff0ef          	jal	ra,12792 <__lockfile>
   133be:	7418                	ld	a4,40(s0)
   133c0:	7c1c                	ld	a5,56(s0)
   133c2:	00f70763          	beq	a4,a5,133d0 <close_file+0x2c>
   133c6:	643c                	ld	a5,72(s0)
   133c8:	4601                	li	a2,0
   133ca:	4581                	li	a1,0
   133cc:	8522                	mv	a0,s0
   133ce:	9782                	jalr	a5
   133d0:	640c                	ld	a1,8(s0)
   133d2:	681c                	ld	a5,16(s0)
   133d4:	00f58b63          	beq	a1,a5,133ea <close_file+0x46>
   133d8:	05043303          	ld	t1,80(s0)
   133dc:	8522                	mv	a0,s0
   133de:	6402                	ld	s0,0(sp)
   133e0:	60a2                	ld	ra,8(sp)
   133e2:	4605                	li	a2,1
   133e4:	8d9d                	sub	a1,a1,a5
   133e6:	0141                	addi	sp,sp,16
   133e8:	8302                	jr	t1
   133ea:	60a2                	ld	ra,8(sp)
   133ec:	6402                	ld	s0,0(sp)
   133ee:	0141                	addi	sp,sp,16
   133f0:	8082                	ret
   133f2:	8082                	ret

00000000000133f4 <__stdio_exit>:
   133f4:	1141                	addi	sp,sp,-16
   133f6:	e022                	sd	s0,0(sp)
   133f8:	e406                	sd	ra,8(sp)
   133fa:	e28ff0ef          	jal	ra,12a22 <__ofl_lock>
   133fe:	6100                	ld	s0,0(a0)
   13400:	ec19                	bnez	s0,1341e <__stdio_exit+0x2a>
   13402:	9501b503          	ld	a0,-1712(gp) # 18150 <__stderr_used>
   13406:	f9fff0ef          	jal	ra,133a4 <close_file>
   1340a:	9281b503          	ld	a0,-1752(gp) # 18128 <__stdout_used>
   1340e:	f97ff0ef          	jal	ra,133a4 <close_file>
   13412:	6402                	ld	s0,0(sp)
   13414:	60a2                	ld	ra,8(sp)
   13416:	9501b503          	ld	a0,-1712(gp) # 18150 <__stderr_used>
   1341a:	0141                	addi	sp,sp,16
   1341c:	b761                	j	133a4 <close_file>
   1341e:	8522                	mv	a0,s0
   13420:	f85ff0ef          	jal	ra,133a4 <close_file>
   13424:	7820                	ld	s0,112(s0)
   13426:	bfe9                	j	13400 <__stdio_exit+0xc>

0000000000013428 <__stdio_write>:
   13428:	715d                	addi	sp,sp,-80
   1342a:	7d1c                	ld	a5,56(a0)
   1342c:	f44e                	sd	s3,40(sp)
   1342e:	02853983          	ld	s3,40(a0)
   13432:	e0a2                	sd	s0,64(sp)
   13434:	fc26                	sd	s1,56(sp)
   13436:	40f989b3          	sub	s3,s3,a5
   1343a:	f84a                	sd	s2,48(sp)
   1343c:	f052                	sd	s4,32(sp)
   1343e:	e44e                	sd	s3,8(sp)
   13440:	e486                	sd	ra,72(sp)
   13442:	842a                	mv	s0,a0
   13444:	8932                	mv	s2,a2
   13446:	e03e                	sd	a5,0(sp)
   13448:	e82e                	sd	a1,16(sp)
   1344a:	ec32                	sd	a2,24(sp)
   1344c:	99b2                	add	s3,s3,a2
   1344e:	4a09                	li	s4,2
   13450:	848a                	mv	s1,sp
   13452:	5c28                	lw	a0,120(s0)
   13454:	04200893          	li	a7,66
   13458:	85a6                	mv	a1,s1
   1345a:	8652                	mv	a2,s4
   1345c:	00000073          	ecall
   13460:	e2bff0ef          	jal	ra,1328a <__syscall_ret>
   13464:	02a99163          	bne	s3,a0,13486 <__stdio_write+0x5e>
   13468:	6c3c                	ld	a5,88(s0)
   1346a:	7038                	ld	a4,96(s0)
   1346c:	fc1c                	sd	a5,56(s0)
   1346e:	973e                	add	a4,a4,a5
   13470:	f018                	sd	a4,32(s0)
   13472:	f41c                	sd	a5,40(s0)
   13474:	60a6                	ld	ra,72(sp)
   13476:	6406                	ld	s0,64(sp)
   13478:	74e2                	ld	s1,56(sp)
   1347a:	79a2                	ld	s3,40(sp)
   1347c:	7a02                	ld	s4,32(sp)
   1347e:	854a                	mv	a0,s2
   13480:	7942                	ld	s2,48(sp)
   13482:	6161                	addi	sp,sp,80
   13484:	8082                	ret
   13486:	02055363          	bgez	a0,134ac <__stdio_write+0x84>
   1348a:	401c                	lw	a5,0(s0)
   1348c:	02043023          	sd	zero,32(s0)
   13490:	02043c23          	sd	zero,56(s0)
   13494:	0207e793          	ori	a5,a5,32
   13498:	c01c                	sw	a5,0(s0)
   1349a:	02043423          	sd	zero,40(s0)
   1349e:	4789                	li	a5,2
   134a0:	02fa0663          	beq	s4,a5,134cc <__stdio_write+0xa4>
   134a4:	649c                	ld	a5,8(s1)
   134a6:	40f90933          	sub	s2,s2,a5
   134aa:	b7e9                	j	13474 <__stdio_write+0x4c>
   134ac:	649c                	ld	a5,8(s1)
   134ae:	40a989b3          	sub	s3,s3,a0
   134b2:	00a7f563          	bgeu	a5,a0,134bc <__stdio_write+0x94>
   134b6:	8d1d                	sub	a0,a0,a5
   134b8:	04c1                	addi	s1,s1,16
   134ba:	3a7d                	addiw	s4,s4,-1
   134bc:	609c                	ld	a5,0(s1)
   134be:	97aa                	add	a5,a5,a0
   134c0:	e09c                	sd	a5,0(s1)
   134c2:	649c                	ld	a5,8(s1)
   134c4:	40a78533          	sub	a0,a5,a0
   134c8:	e488                	sd	a0,8(s1)
   134ca:	b761                	j	13452 <__stdio_write+0x2a>
   134cc:	4901                	li	s2,0
   134ce:	b75d                	j	13474 <__stdio_write+0x4c>

00000000000134d0 <__syscall_cp_c>:
   134d0:	88aa                	mv	a7,a0
   134d2:	852e                	mv	a0,a1
   134d4:	85b2                	mv	a1,a2
   134d6:	8636                	mv	a2,a3
   134d8:	86ba                	mv	a3,a4
   134da:	873e                	mv	a4,a5
   134dc:	87c2                	mv	a5,a6
   134de:	00000073          	ecall
   134e2:	8082                	ret

00000000000134e4 <__syscall_cp>:
   134e4:	b7f5                	j	134d0 <__syscall_cp_c>

00000000000134e6 <__addtf3>:
   134e6:	7179                	addi	sp,sp,-48
   134e8:	f406                	sd	ra,40(sp)
   134ea:	f022                	sd	s0,32(sp)
   134ec:	ec26                	sd	s1,24(sp)
   134ee:	e84a                	sd	s2,16(sp)
   134f0:	e44e                	sd	s3,8(sp)
   134f2:	e052                	sd	s4,0(sp)
   134f4:	82aa                	mv	t0,a0
   134f6:	002024f3          	frrm	s1
   134fa:	59fd                	li	s3,-1
   134fc:	0109d793          	srli	a5,s3,0x10
   13500:	6f21                	lui	t5,0x8
   13502:	00f5fe33          	and	t3,a1,a5
   13506:	ffff0393          	addi	t2,t5,-1 # 7fff <exit-0x8121>
   1350a:	0305d513          	srli	a0,a1,0x30
   1350e:	8ff5                	and	a5,a5,a3
   13510:	0306d313          	srli	t1,a3,0x30
   13514:	078e                	slli	a5,a5,0x3
   13516:	03d65f93          	srli	t6,a2,0x3d
   1351a:	00757433          	and	s0,a0,t2
   1351e:	00737333          	and	t1,t1,t2
   13522:	0e0e                	slli	t3,t3,0x3
   13524:	03d2d713          	srli	a4,t0,0x3d
   13528:	03f5da13          	srli	s4,a1,0x3f
   1352c:	00ffefb3          	or	t6,t6,a5
   13530:	03f6d813          	srli	a6,a3,0x3f
   13534:	406407bb          	subw	a5,s0,t1
   13538:	2481                	sext.w	s1,s1
   1353a:	01c76e33          	or	t3,a4,t3
   1353e:	00329593          	slli	a1,t0,0x3
   13542:	00361e93          	slli	t4,a2,0x3
   13546:	0007889b          	sext.w	a7,a5
   1354a:	1b0a0363          	beq	s4,a6,136f0 <__addtf3+0x20a>
   1354e:	19105163          	blez	a7,136d0 <__addtf3+0x1ea>
   13552:	1e030663          	beqz	t1,1373e <__addtf3+0x258>
   13556:	6c740a63          	beq	s0,t2,13c2a <__addtf3+0x744>
   1355a:	4785                	li	a5,1
   1355c:	17ce                	slli	a5,a5,0x33
   1355e:	00ffefb3          	or	t6,t6,a5
   13562:	07400793          	li	a5,116
   13566:	3f17c963          	blt	a5,a7,13958 <__addtf3+0x472>
   1356a:	03f00793          	li	a5,63
   1356e:	5717c463          	blt	a5,a7,13ad6 <__addtf3+0x5f0>
   13572:	04000793          	li	a5,64
   13576:	411787bb          	subw	a5,a5,a7
   1357a:	011ed733          	srl	a4,t4,a7
   1357e:	00ff9933          	sll	s2,t6,a5
   13582:	00fe9eb3          	sll	t4,t4,a5
   13586:	00e96933          	or	s2,s2,a4
   1358a:	01d03eb3          	snez	t4,t4
   1358e:	011fd8b3          	srl	a7,t6,a7
   13592:	01d96933          	or	s2,s2,t4
   13596:	411e0e33          	sub	t3,t3,a7
   1359a:	41258933          	sub	s2,a1,s2
   1359e:	0125b5b3          	sltu	a1,a1,s2
   135a2:	40be0e33          	sub	t3,t3,a1
   135a6:	00ce1793          	slli	a5,t3,0xc
   135aa:	2a07d363          	bgez	a5,13850 <__addtf3+0x36a>
   135ae:	0e36                	slli	t3,t3,0xd
   135b0:	00de5993          	srli	s3,t3,0xd
   135b4:	36098963          	beqz	s3,13926 <__addtf3+0x440>
   135b8:	854e                	mv	a0,s3
   135ba:	5a9010ef          	jal	ra,15362 <__clzdi2>
   135be:	3551                	addiw	a0,a0,-12
   135c0:	04000e13          	li	t3,64
   135c4:	40ae0e3b          	subw	t3,t3,a0
   135c8:	00a999b3          	sll	s3,s3,a0
   135cc:	01c95e33          	srl	t3,s2,t3
   135d0:	013e6e33          	or	t3,t3,s3
   135d4:	00a919b3          	sll	s3,s2,a0
   135d8:	32854f63          	blt	a0,s0,13916 <__addtf3+0x430>
   135dc:	9d01                	subw	a0,a0,s0
   135de:	0015071b          	addiw	a4,a0,1
   135e2:	03f00793          	li	a5,63
   135e6:	863a                	mv	a2,a4
   135e8:	4ce7c063          	blt	a5,a4,13aa8 <__addtf3+0x5c2>
   135ec:	04000793          	li	a5,64
   135f0:	9f99                	subw	a5,a5,a4
   135f2:	00fe1933          	sll	s2,t3,a5
   135f6:	00e9d633          	srl	a2,s3,a4
   135fa:	00f997b3          	sll	a5,s3,a5
   135fe:	00c96933          	or	s2,s2,a2
   13602:	00f037b3          	snez	a5,a5
   13606:	00f96933          	or	s2,s2,a5
   1360a:	00ee5e33          	srl	t3,t3,a4
   1360e:	01c967b3          	or	a5,s2,t3
   13612:	1a078e63          	beqz	a5,137ce <__addtf3+0x2e8>
   13616:	00797793          	andi	a5,s2,7
   1361a:	00191593          	slli	a1,s2,0x1
   1361e:	001e1613          	slli	a2,t3,0x1
   13622:	03f95713          	srli	a4,s2,0x3f
   13626:	0075f513          	andi	a0,a1,7
   1362a:	9732                	add	a4,a4,a2
   1362c:	4881                	li	a7,0
   1362e:	c911                	beqz	a0,13642 <__addtf3+0x15c>
   13630:	4609                	li	a2,2
   13632:	66c48c63          	beq	s1,a2,13caa <__addtf3+0x7c4>
   13636:	460d                	li	a2,3
   13638:	4885                	li	a7,1
   1363a:	64c48e63          	beq	s1,a2,13c96 <__addtf3+0x7b0>
   1363e:	62048f63          	beqz	s1,13c7c <__addtf3+0x796>
   13642:	4605                	li	a2,1
   13644:	03461593          	slli	a1,a2,0x34
   13648:	8f6d                	and	a4,a4,a1
   1364a:	00173593          	seqz	a1,a4
   1364e:	4401                	li	s0,0
   13650:	6a078563          	beqz	a5,13cfa <__addtf3+0x814>
   13654:	4789                	li	a5,2
   13656:	2af48363          	beq	s1,a5,138fc <__addtf3+0x416>
   1365a:	478d                	li	a5,3
   1365c:	4885                	li	a7,1
   1365e:	2af48963          	beq	s1,a5,13910 <__addtf3+0x42a>
   13662:	28048163          	beqz	s1,138e4 <__addtf3+0x3fe>
   13666:	4605                	li	a2,1
   13668:	164e                	slli	a2,a2,0x33
   1366a:	00ce7633          	and	a2,t3,a2
   1366e:	c199                	beqz	a1,13674 <__addtf3+0x18e>
   13670:	0028e893          	ori	a7,a7,2
   13674:	1a060163          	beqz	a2,13816 <__addtf3+0x330>
   13678:	6721                	lui	a4,0x8
   1367a:	00140793          	addi	a5,s0,1
   1367e:	fff70593          	addi	a1,a4,-1 # 7fff <exit-0x8121>
   13682:	1eb78663          	beq	a5,a1,1386e <__addtf3+0x388>
   13686:	567d                	li	a2,-1
   13688:	03361713          	slli	a4,a2,0x33
   1368c:	177d                	addi	a4,a4,-1
   1368e:	00ee7733          	and	a4,t3,a4
   13692:	8fed                	and	a5,a5,a1
   13694:	03d71513          	slli	a0,a4,0x3d
   13698:	00395913          	srli	s2,s2,0x3
   1369c:	830d                	srli	a4,a4,0x3
   1369e:	8241                	srli	a2,a2,0x10
   136a0:	17c2                	slli	a5,a5,0x30
   136a2:	01256533          	or	a0,a0,s2
   136a6:	8f71                	and	a4,a4,a2
   136a8:	93c1                	srli	a5,a5,0x30
   136aa:	86d2                	mv	a3,s4
   136ac:	06be                	slli	a3,a3,0xf
   136ae:	01071593          	slli	a1,a4,0x10
   136b2:	00f6e733          	or	a4,a3,a5
   136b6:	1742                	slli	a4,a4,0x30
   136b8:	81c1                	srli	a1,a1,0x10
   136ba:	8dd9                	or	a1,a1,a4
   136bc:	2a089c63          	bnez	a7,13974 <__addtf3+0x48e>
   136c0:	70a2                	ld	ra,40(sp)
   136c2:	7402                	ld	s0,32(sp)
   136c4:	64e2                	ld	s1,24(sp)
   136c6:	6942                	ld	s2,16(sp)
   136c8:	69a2                	ld	s3,8(sp)
   136ca:	6a02                	ld	s4,0(sp)
   136cc:	6145                	addi	sp,sp,48
   136ce:	8082                	ret
   136d0:	0a088663          	beqz	a7,1377c <__addtf3+0x296>
   136d4:	32041663          	bnez	s0,13a00 <__addtf3+0x51a>
   136d8:	00be6733          	or	a4,t3,a1
   136dc:	16070463          	beqz	a4,13844 <__addtf3+0x35e>
   136e0:	6f388d63          	beq	a7,s3,13dda <__addtf3+0x8f4>
   136e4:	5e730663          	beq	t1,t2,13cd0 <__addtf3+0x7ea>
   136e8:	fff7c793          	not	a5,a5
   136ec:	2781                	sext.w	a5,a5
   136ee:	a60d                	j	13a10 <__addtf3+0x52a>
   136f0:	19105e63          	blez	a7,1388c <__addtf3+0x3a6>
   136f4:	0e030863          	beqz	t1,137e4 <__addtf3+0x2fe>
   136f8:	04740d63          	beq	s0,t2,13752 <__addtf3+0x26c>
   136fc:	4785                	li	a5,1
   136fe:	17ce                	slli	a5,a5,0x33
   13700:	00ffefb3          	or	t6,t6,a5
   13704:	07400793          	li	a5,116
   13708:	3517cb63          	blt	a5,a7,13a5e <__addtf3+0x578>
   1370c:	03f00793          	li	a5,63
   13710:	4317db63          	bge	a5,a7,13b46 <__addtf3+0x660>
   13714:	fc08891b          	addiw	s2,a7,-64
   13718:	04000793          	li	a5,64
   1371c:	012fd733          	srl	a4,t6,s2
   13720:	00f88a63          	beq	a7,a5,13734 <__addtf3+0x24e>
   13724:	08000793          	li	a5,128
   13728:	411787bb          	subw	a5,a5,a7
   1372c:	00ff97b3          	sll	a5,t6,a5
   13730:	00feeeb3          	or	t4,t4,a5
   13734:	01d03933          	snez	s2,t4
   13738:	00e96933          	or	s2,s2,a4
   1373c:	a62d                	j	13a66 <__addtf3+0x580>
   1373e:	01dfe733          	or	a4,t6,t4
   13742:	30070363          	beqz	a4,13a48 <__addtf3+0x562>
   13746:	fff7889b          	addiw	a7,a5,-1
   1374a:	4c088763          	beqz	a7,13c18 <__addtf3+0x732>
   1374e:	e0741ae3          	bne	s0,t2,13562 <__addtf3+0x7c>
   13752:	00be65b3          	or	a1,t3,a1
   13756:	4a058f63          	beqz	a1,13c14 <__addtf3+0x72e>
   1375a:	032e5893          	srli	a7,t3,0x32
   1375e:	0039d713          	srli	a4,s3,0x3
   13762:	0018f893          	andi	a7,a7,1
   13766:	03de1613          	slli	a2,t3,0x3d
   1376a:	00e2f533          	and	a0,t0,a4
   1376e:	0018b893          	seqz	a7,a7
   13772:	8d51                	or	a0,a0,a2
   13774:	003e5793          	srli	a5,t3,0x3
   13778:	0892                	slli	a7,a7,0x4
   1377a:	a855                	j	1382e <__addtf3+0x348>
   1377c:	00140793          	addi	a5,s0,1
   13780:	1f79                	addi	t5,t5,-2
   13782:	01e7ff33          	and	t5,a5,t5
   13786:	240f1663          	bnez	t5,139d2 <__addtf3+0x4ec>
   1378a:	00be6733          	or	a4,t3,a1
   1378e:	01dfe7b3          	or	a5,t6,t4
   13792:	40041063          	bnez	s0,13b92 <__addtf3+0x6ac>
   13796:	4a070e63          	beqz	a4,13c52 <__addtf3+0x76c>
   1379a:	4c078263          	beqz	a5,13c5e <__addtf3+0x778>
   1379e:	41d58633          	sub	a2,a1,t4
   137a2:	00c5b7b3          	sltu	a5,a1,a2
   137a6:	41fe0733          	sub	a4,t3,t6
   137aa:	8f1d                	sub	a4,a4,a5
   137ac:	00c71793          	slli	a5,a4,0xc
   137b0:	6a07d663          	bgez	a5,13e5c <__addtf3+0x976>
   137b4:	40be8933          	sub	s2,t4,a1
   137b8:	41cf8733          	sub	a4,t6,t3
   137bc:	012ebe33          	sltu	t3,t4,s2
   137c0:	41c70e33          	sub	t3,a4,t3
   137c4:	01c967b3          	or	a5,s2,t3
   137c8:	8a42                	mv	s4,a6
   137ca:	e40796e3          	bnez	a5,13616 <__addtf3+0x130>
   137ce:	4501                	li	a0,0
   137d0:	4401                	li	s0,0
   137d2:	4881                	li	a7,0
   137d4:	01079713          	slli	a4,a5,0x10
   137d8:	03141793          	slli	a5,s0,0x31
   137dc:	8341                	srli	a4,a4,0x10
   137de:	93c5                	srli	a5,a5,0x31
   137e0:	86d2                	mv	a3,s4
   137e2:	b5e9                	j	136ac <__addtf3+0x1c6>
   137e4:	01dfe733          	or	a4,t6,t4
   137e8:	46070d63          	beqz	a4,13c62 <__addtf3+0x77c>
   137ec:	fff7889b          	addiw	a7,a5,-1
   137f0:	3c088c63          	beqz	a7,13bc8 <__addtf3+0x6e2>
   137f4:	f07418e3          	bne	s0,t2,13704 <__addtf3+0x21e>
   137f8:	00be67b3          	or	a5,t3,a1
   137fc:	40078c63          	beqz	a5,13c14 <__addtf3+0x72e>
   13800:	032e5893          	srli	a7,t3,0x32
   13804:	0018f893          	andi	a7,a7,1
   13808:	0018b893          	seqz	a7,a7
   1380c:	0892                	slli	a7,a7,0x4
   1380e:	67a1                	lui	a5,0x8
   13810:	892e                	mv	s2,a1
   13812:	fff78413          	addi	s0,a5,-1 # 7fff <exit-0x8121>
   13816:	6721                	lui	a4,0x8
   13818:	03de1513          	slli	a0,t3,0x3d
   1381c:	00395913          	srli	s2,s2,0x3
   13820:	177d                	addi	a4,a4,-1
   13822:	01256533          	or	a0,a0,s2
   13826:	003e5793          	srli	a5,t3,0x3
   1382a:	fae415e3          	bne	s0,a4,137d4 <__addtf3+0x2ee>
   1382e:	8d5d                	or	a0,a0,a5
   13830:	10050d63          	beqz	a0,1394a <__addtf3+0x464>
   13834:	6721                	lui	a4,0x8
   13836:	fff70793          	addi	a5,a4,-1 # 7fff <exit-0x8121>
   1383a:	4705                	li	a4,1
   1383c:	4681                	li	a3,0
   1383e:	173e                	slli	a4,a4,0x2f
   13840:	4501                	li	a0,0
   13842:	b5ad                	j	136ac <__addtf3+0x1c6>
   13844:	5c730563          	beq	t1,t2,13e0e <__addtf3+0x928>
   13848:	8e7e                	mv	t3,t6
   1384a:	8976                	mv	s2,t4
   1384c:	841a                	mv	s0,t1
   1384e:	8a42                	mv	s4,a6
   13850:	01c967b3          	or	a5,s2,t3
   13854:	da040fe3          	beqz	s0,13612 <__addtf3+0x12c>
   13858:	00797793          	andi	a5,s2,7
   1385c:	4581                	li	a1,0
   1385e:	de079be3          	bnez	a5,13654 <__addtf3+0x16e>
   13862:	4605                	li	a2,1
   13864:	164e                	slli	a2,a2,0x33
   13866:	00ce7633          	and	a2,t3,a2
   1386a:	4881                	li	a7,0
   1386c:	b521                	j	13674 <__addtf3+0x18e>
   1386e:	cce1                	beqz	s1,13946 <__addtf3+0x460>
   13870:	478d                	li	a5,3
   13872:	2cf48063          	beq	s1,a5,13b32 <__addtf3+0x64c>
   13876:	4789                	li	a5,2
   13878:	0cf48563          	beq	s1,a5,13942 <__addtf3+0x45c>
   1387c:	557d                	li	a0,-1
   1387e:	6421                	lui	s0,0x8
   13880:	0058e893          	ori	a7,a7,5
   13884:	00355793          	srli	a5,a0,0x3
   13888:	1479                	addi	s0,s0,-2
   1388a:	b7a9                	j	137d4 <__addtf3+0x2ee>
   1388c:	0e088e63          	beqz	a7,13988 <__addtf3+0x4a2>
   13890:	28040563          	beqz	s0,13b1a <__addtf3+0x634>
   13894:	4c730a63          	beq	t1,t2,13d68 <__addtf3+0x882>
   13898:	4705                	li	a4,1
   1389a:	174e                	slli	a4,a4,0x33
   1389c:	40f007bb          	negw	a5,a5
   138a0:	00ee6e33          	or	t3,t3,a4
   138a4:	07400713          	li	a4,116
   138a8:	46f74663          	blt	a4,a5,13d14 <__addtf3+0x82e>
   138ac:	03f00713          	li	a4,63
   138b0:	50f74063          	blt	a4,a5,13db0 <__addtf3+0x8ca>
   138b4:	04000713          	li	a4,64
   138b8:	9f1d                	subw	a4,a4,a5
   138ba:	00ee1933          	sll	s2,t3,a4
   138be:	00f5d633          	srl	a2,a1,a5
   138c2:	00e59733          	sll	a4,a1,a4
   138c6:	00c96933          	or	s2,s2,a2
   138ca:	00e03733          	snez	a4,a4
   138ce:	00fe57b3          	srl	a5,t3,a5
   138d2:	00e96933          	or	s2,s2,a4
   138d6:	9fbe                	add	t6,t6,a5
   138d8:	9976                	add	s2,s2,t4
   138da:	01d93e33          	sltu	t3,s2,t4
   138de:	9e7e                	add	t3,t3,t6
   138e0:	841a                	mv	s0,t1
   138e2:	a271                	j	13a6e <__addtf3+0x588>
   138e4:	00f97793          	andi	a5,s2,15
   138e8:	4711                	li	a4,4
   138ea:	d6e78ee3          	beq	a5,a4,13666 <__addtf3+0x180>
   138ee:	00490793          	addi	a5,s2,4
   138f2:	0127b933          	sltu	s2,a5,s2
   138f6:	9e4a                	add	t3,t3,s2
   138f8:	893e                	mv	s2,a5
   138fa:	b3b5                	j	13666 <__addtf3+0x180>
   138fc:	4885                	li	a7,1
   138fe:	d60a04e3          	beqz	s4,13666 <__addtf3+0x180>
   13902:	00890793          	addi	a5,s2,8
   13906:	0127b933          	sltu	s2,a5,s2
   1390a:	9e4a                	add	t3,t3,s2
   1390c:	893e                	mv	s2,a5
   1390e:	bba1                	j	13666 <__addtf3+0x180>
   13910:	fe0a09e3          	beqz	s4,13902 <__addtf3+0x41c>
   13914:	bb89                	j	13666 <__addtf3+0x180>
   13916:	577d                	li	a4,-1
   13918:	174e                	slli	a4,a4,0x33
   1391a:	177d                	addi	a4,a4,-1
   1391c:	8c09                	sub	s0,s0,a0
   1391e:	00ee7e33          	and	t3,t3,a4
   13922:	894e                	mv	s2,s3
   13924:	b735                	j	13850 <__addtf3+0x36a>
   13926:	854a                	mv	a0,s2
   13928:	23b010ef          	jal	ra,15362 <__clzdi2>
   1392c:	ff450e1b          	addiw	t3,a0,-12
   13930:	03f00793          	li	a5,63
   13934:	0345051b          	addiw	a0,a0,52
   13938:	01c91e33          	sll	t3,s2,t3
   1393c:	c8a7cee3          	blt	a5,a0,135d8 <__addtf3+0xf2>
   13940:	b141                	j	135c0 <__addtf3+0xda>
   13942:	1e0a0a63          	beqz	s4,13b36 <__addtf3+0x650>
   13946:	0058e893          	ori	a7,a7,5
   1394a:	6721                	lui	a4,0x8
   1394c:	fff70793          	addi	a5,a4,-1 # 7fff <exit-0x8121>
   13950:	86d2                	mv	a3,s4
   13952:	4701                	li	a4,0
   13954:	4501                	li	a0,0
   13956:	bb99                	j	136ac <__addtf3+0x1c6>
   13958:	01dfe933          	or	s2,t6,t4
   1395c:	01203933          	snez	s2,s2
   13960:	b92d                	j	1359a <__addtf3+0xb4>
   13962:	28830663          	beq	t1,s0,13bee <__addtf3+0x708>
   13966:	28079c63          	bnez	a5,13bfe <__addtf3+0x718>
   1396a:	0ffff5b7          	lui	a1,0xffff
   1396e:	4501                	li	a0,0
   13970:	158e                	slli	a1,a1,0x23
   13972:	48c1                	li	a7,16
   13974:	0018a073          	csrs	fflags,a7
   13978:	70a2                	ld	ra,40(sp)
   1397a:	7402                	ld	s0,32(sp)
   1397c:	64e2                	ld	s1,24(sp)
   1397e:	6942                	ld	s2,16(sp)
   13980:	69a2                	ld	s3,8(sp)
   13982:	6a02                	ld	s4,0(sp)
   13984:	6145                	addi	sp,sp,48
   13986:	8082                	ret
   13988:	00140713          	addi	a4,s0,1 # 8001 <exit-0x811f>
   1398c:	1f79                	addi	t5,t5,-2
   1398e:	01e777b3          	and	a5,a4,t5
   13992:	1c079e63          	bnez	a5,13b6e <__addtf3+0x688>
   13996:	00be6733          	or	a4,t3,a1
   1399a:	3a041763          	bnez	s0,13d48 <__addtf3+0x862>
   1399e:	01dfe7b3          	or	a5,t6,t4
   139a2:	40070463          	beqz	a4,13daa <__addtf3+0x8c4>
   139a6:	2a078c63          	beqz	a5,13c5e <__addtf3+0x778>
   139aa:	01d58933          	add	s2,a1,t4
   139ae:	9e7e                	add	t3,t3,t6
   139b0:	00b935b3          	sltu	a1,s2,a1
   139b4:	9e2e                	add	t3,t3,a1
   139b6:	00ce1793          	slli	a5,t3,0xc
   139ba:	c407dae3          	bgez	a5,1360e <__addtf3+0x128>
   139be:	03399713          	slli	a4,s3,0x33
   139c2:	fff70793          	addi	a5,a4,-1
   139c6:	00fe7e33          	and	t3,t3,a5
   139ca:	4405                	li	s0,1
   139cc:	00797793          	andi	a5,s2,7
   139d0:	b571                	j	1385c <__addtf3+0x376>
   139d2:	41d58933          	sub	s2,a1,t4
   139d6:	0125b7b3          	sltu	a5,a1,s2
   139da:	41fe09b3          	sub	s3,t3,t6
   139de:	40f989b3          	sub	s3,s3,a5
   139e2:	00c99793          	slli	a5,s3,0xc
   139e6:	1c07c763          	bltz	a5,13bb4 <__addtf3+0x6ce>
   139ea:	013967b3          	or	a5,s2,s3
   139ee:	bc0793e3          	bnez	a5,135b4 <__addtf3+0xce>
   139f2:	ffe48693          	addi	a3,s1,-2
   139f6:	0016ba13          	seqz	s4,a3
   139fa:	4501                	li	a0,0
   139fc:	4401                	li	s0,0
   139fe:	bbd9                	j	137d4 <__addtf3+0x2ee>
   13a00:	2c730863          	beq	t1,t2,13cd0 <__addtf3+0x7ea>
   13a04:	4705                	li	a4,1
   13a06:	174e                	slli	a4,a4,0x33
   13a08:	40f007bb          	negw	a5,a5
   13a0c:	00ee6e33          	or	t3,t3,a4
   13a10:	07400713          	li	a4,116
   13a14:	0ef74663          	blt	a4,a5,13b00 <__addtf3+0x61a>
   13a18:	03f00713          	li	a4,63
   13a1c:	30f74163          	blt	a4,a5,13d1e <__addtf3+0x838>
   13a20:	04000713          	li	a4,64
   13a24:	9f1d                	subw	a4,a4,a5
   13a26:	00f5d6b3          	srl	a3,a1,a5
   13a2a:	00ee1933          	sll	s2,t3,a4
   13a2e:	00e595b3          	sll	a1,a1,a4
   13a32:	00d96933          	or	s2,s2,a3
   13a36:	00b035b3          	snez	a1,a1
   13a3a:	00fe57b3          	srl	a5,t3,a5
   13a3e:	00b96933          	or	s2,s2,a1
   13a42:	40ff8fb3          	sub	t6,t6,a5
   13a46:	a0c9                	j	13b08 <__addtf3+0x622>
   13a48:	892e                	mv	s2,a1
   13a4a:	e07413e3          	bne	s0,t2,13850 <__addtf3+0x36a>
   13a4e:	00be67b3          	or	a5,t3,a1
   13a52:	da0797e3          	bnez	a5,13800 <__addtf3+0x31a>
   13a56:	4501                	li	a0,0
   13a58:	4781                	li	a5,0
   13a5a:	4881                	li	a7,0
   13a5c:	bbc9                	j	1382e <__addtf3+0x348>
   13a5e:	01dfe933          	or	s2,t6,t4
   13a62:	01203933          	snez	s2,s2
   13a66:	992e                	add	s2,s2,a1
   13a68:	00b935b3          	sltu	a1,s2,a1
   13a6c:	9e2e                	add	t3,t3,a1
   13a6e:	00ce1793          	slli	a5,t3,0xc
   13a72:	dc07dfe3          	bgez	a5,13850 <__addtf3+0x36a>
   13a76:	67a1                	lui	a5,0x8
   13a78:	0405                	addi	s0,s0,1
   13a7a:	fff78713          	addi	a4,a5,-1 # 7fff <exit-0x8121>
   13a7e:	22e40a63          	beq	s0,a4,13cb2 <__addtf3+0x7cc>
   13a82:	577d                	li	a4,-1
   13a84:	174e                	slli	a4,a4,0x33
   13a86:	177d                	addi	a4,a4,-1
   13a88:	00ee7e33          	and	t3,t3,a4
   13a8c:	00195793          	srli	a5,s2,0x1
   13a90:	00197913          	andi	s2,s2,1
   13a94:	0127e7b3          	or	a5,a5,s2
   13a98:	03fe1913          	slli	s2,t3,0x3f
   13a9c:	00f96933          	or	s2,s2,a5
   13aa0:	001e5e13          	srli	t3,t3,0x1
   13aa4:	8b9d                	andi	a5,a5,7
   13aa6:	bb5d                	j	1385c <__addtf3+0x376>
   13aa8:	fc15079b          	addiw	a5,a0,-63
   13aac:	04000593          	li	a1,64
   13ab0:	00fe57b3          	srl	a5,t3,a5
   13ab4:	00b70a63          	beq	a4,a1,13ac8 <__addtf3+0x5e2>
   13ab8:	08000713          	li	a4,128
   13abc:	40c7063b          	subw	a2,a4,a2
   13ac0:	00ce1e33          	sll	t3,t3,a2
   13ac4:	01c9e9b3          	or	s3,s3,t3
   13ac8:	013039b3          	snez	s3,s3
   13acc:	00f9e7b3          	or	a5,s3,a5
   13ad0:	893e                	mv	s2,a5
   13ad2:	4e01                	li	t3,0
   13ad4:	be3d                	j	13612 <__addtf3+0x12c>
   13ad6:	fc08891b          	addiw	s2,a7,-64
   13ada:	04000793          	li	a5,64
   13ade:	012fd733          	srl	a4,t6,s2
   13ae2:	00f88a63          	beq	a7,a5,13af6 <__addtf3+0x610>
   13ae6:	08000793          	li	a5,128
   13aea:	411787bb          	subw	a5,a5,a7
   13aee:	00ff97b3          	sll	a5,t6,a5
   13af2:	00feeeb3          	or	t4,t4,a5
   13af6:	01d03933          	snez	s2,t4
   13afa:	00e96933          	or	s2,s2,a4
   13afe:	bc71                	j	1359a <__addtf3+0xb4>
   13b00:	00be6e33          	or	t3,t3,a1
   13b04:	01c03933          	snez	s2,t3
   13b08:	412e8933          	sub	s2,t4,s2
   13b0c:	012ebe33          	sltu	t3,t4,s2
   13b10:	41cf8e33          	sub	t3,t6,t3
   13b14:	841a                	mv	s0,t1
   13b16:	8a42                	mv	s4,a6
   13b18:	b479                	j	135a6 <__addtf3+0xc0>
   13b1a:	00be6733          	or	a4,t3,a1
   13b1e:	26070a63          	beqz	a4,13d92 <__addtf3+0x8ac>
   13b22:	39388663          	beq	a7,s3,13eae <__addtf3+0x9c8>
   13b26:	24730163          	beq	t1,t2,13d68 <__addtf3+0x882>
   13b2a:	fff7c793          	not	a5,a5
   13b2e:	2781                	sext.w	a5,a5
   13b30:	bb95                	j	138a4 <__addtf3+0x3be>
   13b32:	e00a0ae3          	beqz	s4,13946 <__addtf3+0x460>
   13b36:	557d                	li	a0,-1
   13b38:	0058e893          	ori	a7,a7,5
   13b3c:	00355793          	srli	a5,a0,0x3
   13b40:	ffe70413          	addi	s0,a4,-2
   13b44:	b941                	j	137d4 <__addtf3+0x2ee>
   13b46:	04000793          	li	a5,64
   13b4a:	411787bb          	subw	a5,a5,a7
   13b4e:	011ed733          	srl	a4,t4,a7
   13b52:	00ff9933          	sll	s2,t6,a5
   13b56:	00fe9eb3          	sll	t4,t4,a5
   13b5a:	00e96933          	or	s2,s2,a4
   13b5e:	01d03eb3          	snez	t4,t4
   13b62:	011fd8b3          	srl	a7,t6,a7
   13b66:	01d96933          	or	s2,s2,t4
   13b6a:	9e46                	add	t3,t3,a7
   13b6c:	bded                	j	13a66 <__addtf3+0x580>
   13b6e:	28770263          	beq	a4,t2,13df2 <__addtf3+0x90c>
   13b72:	01d587b3          	add	a5,a1,t4
   13b76:	00b7b5b3          	sltu	a1,a5,a1
   13b7a:	9e7e                	add	t3,t3,t6
   13b7c:	9e2e                	add	t3,t3,a1
   13b7e:	8385                	srli	a5,a5,0x1
   13b80:	03fe1913          	slli	s2,t3,0x3f
   13b84:	00f96933          	or	s2,s2,a5
   13b88:	001e5e13          	srli	t3,t3,0x1
   13b8c:	8b9d                	andi	a5,a5,7
   13b8e:	843a                	mv	s0,a4
   13b90:	b1f1                	j	1385c <__addtf3+0x376>
   13b92:	04740363          	beq	s0,t2,13bd8 <__addtf3+0x6f2>
   13b96:	04730c63          	beq	t1,t2,13bee <__addtf3+0x708>
   13b9a:	dc0706e3          	beqz	a4,13966 <__addtf3+0x480>
   13b9e:	1a079e63          	bnez	a5,13d5a <__addtf3+0x874>
   13ba2:	00329793          	slli	a5,t0,0x3
   13ba6:	838d                	srli	a5,a5,0x3
   13ba8:	03de1513          	slli	a0,t3,0x3d
   13bac:	8d5d                	or	a0,a0,a5
   13bae:	003e5793          	srli	a5,t3,0x3
   13bb2:	b9b5                	j	1382e <__addtf3+0x348>
   13bb4:	40be8933          	sub	s2,t4,a1
   13bb8:	41cf8e33          	sub	t3,t6,t3
   13bbc:	012ebeb3          	sltu	t4,t4,s2
   13bc0:	41de09b3          	sub	s3,t3,t4
   13bc4:	8a42                	mv	s4,a6
   13bc6:	b2fd                	j	135b4 <__addtf3+0xce>
   13bc8:	01d58933          	add	s2,a1,t4
   13bcc:	01fe0733          	add	a4,t3,t6
   13bd0:	00b93e33          	sltu	t3,s2,a1
   13bd4:	9e3a                	add	t3,t3,a4
   13bd6:	bd61                	j	13a6e <__addtf3+0x588>
   13bd8:	d80705e3          	beqz	a4,13962 <__addtf3+0x47c>
   13bdc:	00de1693          	slli	a3,t3,0xd
   13be0:	0006c363          	bltz	a3,13be6 <__addtf3+0x700>
   13be4:	48c1                	li	a7,16
   13be6:	65a1                	lui	a1,0x8
   13be8:	15fd                	addi	a1,a1,-1
   13bea:	fab31ae3          	bne	t1,a1,13b9e <__addtf3+0x6b8>
   13bee:	d7d5                	beqz	a5,13b9a <__addtf3+0x6b4>
   13bf0:	00df9793          	slli	a5,t6,0xd
   13bf4:	0007c363          	bltz	a5,13bfa <__addtf3+0x714>
   13bf8:	48c1                	li	a7,16
   13bfa:	16071063          	bnez	a4,13d5a <__addtf3+0x874>
   13bfe:	00361793          	slli	a5,a2,0x3
   13c02:	838d                	srli	a5,a5,0x3
   13c04:	03df9513          	slli	a0,t6,0x3d
   13c08:	8d5d                	or	a0,a0,a5
   13c0a:	8a42                	mv	s4,a6
   13c0c:	003fd793          	srli	a5,t6,0x3
   13c10:	b939                	j	1382e <__addtf3+0x348>
   13c12:	8a42                	mv	s4,a6
   13c14:	4881                	li	a7,0
   13c16:	bb15                	j	1394a <__addtf3+0x464>
   13c18:	41d58933          	sub	s2,a1,t4
   13c1c:	41fe0733          	sub	a4,t3,t6
   13c20:	0125be33          	sltu	t3,a1,s2
   13c24:	41c70e33          	sub	t3,a4,t3
   13c28:	babd                	j	135a6 <__addtf3+0xc0>
   13c2a:	00be65b3          	or	a1,t3,a1
   13c2e:	d1fd                	beqz	a1,13c14 <__addtf3+0x72e>
   13c30:	032e5893          	srli	a7,t3,0x32
   13c34:	0039d613          	srli	a2,s3,0x3
   13c38:	0018f893          	andi	a7,a7,1
   13c3c:	03de1793          	slli	a5,t3,0x3d
   13c40:	00c2f533          	and	a0,t0,a2
   13c44:	0018b893          	seqz	a7,a7
   13c48:	8d5d                	or	a0,a0,a5
   13c4a:	0892                	slli	a7,a7,0x4
   13c4c:	003e5793          	srli	a5,t3,0x3
   13c50:	bef9                	j	1382e <__addtf3+0x348>
   13c52:	14078663          	beqz	a5,13d9e <__addtf3+0x8b8>
   13c56:	4781                	li	a5,0
   13c58:	8e7e                	mv	t3,t6
   13c5a:	85f6                	mv	a1,t4
   13c5c:	8a42                	mv	s4,a6
   13c5e:	892e                	mv	s2,a1
   13c60:	ba6d                	j	1361a <__addtf3+0x134>
   13c62:	892e                	mv	s2,a1
   13c64:	be7416e3          	bne	s0,t2,13850 <__addtf3+0x36a>
   13c68:	00be67b3          	or	a5,t3,a1
   13c6c:	de0785e3          	beqz	a5,13a56 <__addtf3+0x570>
   13c70:	032e5893          	srli	a7,t3,0x32
   13c74:	0018b893          	seqz	a7,a7
   13c78:	0892                	slli	a7,a7,0x4
   13c7a:	be51                	j	1380e <__addtf3+0x328>
   13c7c:	00f5f613          	andi	a2,a1,15
   13c80:	4511                	li	a0,4
   13c82:	9ca600e3          	beq	a2,a0,13642 <__addtf3+0x15c>
   13c86:	ffc5b613          	sltiu	a2,a1,-4
   13c8a:	00164613          	xori	a2,a2,1
   13c8e:	1602                	slli	a2,a2,0x20
   13c90:	9201                	srli	a2,a2,0x20
   13c92:	9732                	add	a4,a4,a2
   13c94:	b27d                	j	13642 <__addtf3+0x15c>
   13c96:	9a0a16e3          	bnez	s4,13642 <__addtf3+0x15c>
   13c9a:	ff85b613          	sltiu	a2,a1,-8
   13c9e:	00164613          	xori	a2,a2,1
   13ca2:	1602                	slli	a2,a2,0x20
   13ca4:	9201                	srli	a2,a2,0x20
   13ca6:	9732                	add	a4,a4,a2
   13ca8:	ba69                	j	13642 <__addtf3+0x15c>
   13caa:	4885                	li	a7,1
   13cac:	980a0be3          	beqz	s4,13642 <__addtf3+0x15c>
   13cb0:	b7ed                	j	13c9a <__addtf3+0x7b4>
   13cb2:	ccb9                	beqz	s1,13d10 <__addtf3+0x82a>
   13cb4:	470d                	li	a4,3
   13cb6:	1ee48263          	beq	s1,a4,13e9a <__addtf3+0x9b4>
   13cba:	4709                	li	a4,2
   13cbc:	4895                	li	a7,5
   13cbe:	bae49fe3          	bne	s1,a4,1387c <__addtf3+0x396>
   13cc2:	c80a14e3          	bnez	s4,1394a <__addtf3+0x464>
   13cc6:	ffe78413          	addi	s0,a5,-2
   13cca:	5e7d                	li	t3,-1
   13ccc:	597d                	li	s2,-1
   13cce:	b26d                	j	13678 <__addtf3+0x192>
   13cd0:	01dfeeb3          	or	t4,t6,t4
   13cd4:	f20e8fe3          	beqz	t4,13c12 <__addtf3+0x72c>
   13cd8:	0039d713          	srli	a4,s3,0x3
   13cdc:	00e677b3          	and	a5,a2,a4
   13ce0:	03df9513          	slli	a0,t6,0x3d
   13ce4:	00df9713          	slli	a4,t6,0xd
   13ce8:	8d5d                	or	a0,a0,a5
   13cea:	8a42                	mv	s4,a6
   13cec:	003fd793          	srli	a5,t6,0x3
   13cf0:	48c1                	li	a7,16
   13cf2:	b2075ee3          	bgez	a4,1382e <__addtf3+0x348>
   13cf6:	4881                	li	a7,0
   13cf8:	be1d                	j	1382e <__addtf3+0x348>
   13cfa:	164e                	slli	a2,a2,0x33
   13cfc:	00ce7633          	and	a2,t3,a2
   13d00:	96071ae3          	bnez	a4,13674 <__addtf3+0x18e>
   13d04:	843e                	mv	s0,a5
   13d06:	960887e3          	beqz	a7,13674 <__addtf3+0x18e>
   13d0a:	0028e893          	ori	a7,a7,2
   13d0e:	b29d                	j	13674 <__addtf3+0x18e>
   13d10:	4895                	li	a7,5
   13d12:	b925                	j	1394a <__addtf3+0x464>
   13d14:	00be6e33          	or	t3,t3,a1
   13d18:	01c03933          	snez	s2,t3
   13d1c:	be75                	j	138d8 <__addtf3+0x3f2>
   13d1e:	fc07891b          	addiw	s2,a5,-64
   13d22:	04000693          	li	a3,64
   13d26:	012e5733          	srl	a4,t3,s2
   13d2a:	00d78a63          	beq	a5,a3,13d3e <__addtf3+0x858>
   13d2e:	08000693          	li	a3,128
   13d32:	40f687bb          	subw	a5,a3,a5
   13d36:	00fe1e33          	sll	t3,t3,a5
   13d3a:	01c5e5b3          	or	a1,a1,t3
   13d3e:	00b03933          	snez	s2,a1
   13d42:	00e96933          	or	s2,s2,a4
   13d46:	b3c9                	j	13b08 <__addtf3+0x622>
   13d48:	12740463          	beq	s0,t2,13e70 <__addtf3+0x98a>
   13d4c:	0e730963          	beq	t1,t2,13e3e <__addtf3+0x958>
   13d50:	cf6d                	beqz	a4,13e4a <__addtf3+0x964>
   13d52:	01dfe7b3          	or	a5,t6,t4
   13d56:	e40786e3          	beqz	a5,13ba2 <__addtf3+0x6bc>
   13d5a:	67a1                	lui	a5,0x8
   13d5c:	4705                	li	a4,1
   13d5e:	4681                	li	a3,0
   13d60:	17fd                	addi	a5,a5,-1
   13d62:	173e                	slli	a4,a4,0x2f
   13d64:	4501                	li	a0,0
   13d66:	b299                	j	136ac <__addtf3+0x1c6>
   13d68:	01dfeeb3          	or	t4,t6,t4
   13d6c:	ea0e84e3          	beqz	t4,13c14 <__addtf3+0x72e>
   13d70:	032fd893          	srli	a7,t6,0x32
   13d74:	0039d713          	srli	a4,s3,0x3
   13d78:	0018f893          	andi	a7,a7,1
   13d7c:	03df9793          	slli	a5,t6,0x3d
   13d80:	00e67533          	and	a0,a2,a4
   13d84:	0018b893          	seqz	a7,a7
   13d88:	8d5d                	or	a0,a0,a5
   13d8a:	0892                	slli	a7,a7,0x4
   13d8c:	003fd793          	srli	a5,t6,0x3
   13d90:	bc79                	j	1382e <__addtf3+0x348>
   13d92:	08730a63          	beq	t1,t2,13e26 <__addtf3+0x940>
   13d96:	8e7e                	mv	t3,t6
   13d98:	8976                	mv	s2,t4
   13d9a:	841a                	mv	s0,t1
   13d9c:	bc55                	j	13850 <__addtf3+0x36a>
   13d9e:	ffe48693          	addi	a3,s1,-2
   13da2:	0016ba13          	seqz	s4,a3
   13da6:	4501                	li	a0,0
   13da8:	b435                	j	137d4 <__addtf3+0x2ee>
   13daa:	8e7e                	mv	t3,t6
   13dac:	8976                	mv	s2,t4
   13dae:	b095                	j	13612 <__addtf3+0x12c>
   13db0:	fc07871b          	addiw	a4,a5,-64
   13db4:	04000613          	li	a2,64
   13db8:	00ee5733          	srl	a4,t3,a4
   13dbc:	00c78a63          	beq	a5,a2,13dd0 <__addtf3+0x8ea>
   13dc0:	08000613          	li	a2,128
   13dc4:	40f607bb          	subw	a5,a2,a5
   13dc8:	00fe1e33          	sll	t3,t3,a5
   13dcc:	01c5e5b3          	or	a1,a1,t3
   13dd0:	00b03933          	snez	s2,a1
   13dd4:	00e96933          	or	s2,s2,a4
   13dd8:	b601                	j	138d8 <__addtf3+0x3f2>
   13dda:	40be8933          	sub	s2,t4,a1
   13dde:	41cf8733          	sub	a4,t6,t3
   13de2:	012ebe33          	sltu	t3,t4,s2
   13de6:	41c70e33          	sub	t3,a4,t3
   13dea:	841a                	mv	s0,t1
   13dec:	8a42                	mv	s4,a6
   13dee:	fb8ff06f          	j	135a6 <__addtf3+0xc0>
   13df2:	dc99                	beqz	s1,13d10 <__addtf3+0x82a>
   13df4:	478d                	li	a5,3
   13df6:	0cf48e63          	beq	s1,a5,13ed2 <__addtf3+0x9ec>
   13dfa:	4789                	li	a5,2
   13dfc:	4895                	li	a7,5
   13dfe:	a6f49fe3          	bne	s1,a5,1387c <__addtf3+0x396>
   13e02:	b40a14e3          	bnez	s4,1394a <__addtf3+0x464>
   13e06:	847a                	mv	s0,t5
   13e08:	5e7d                	li	t3,-1
   13e0a:	597d                	li	s2,-1
   13e0c:	b0b5                	j	13678 <__addtf3+0x192>
   13e0e:	01dfe7b3          	or	a5,t6,t4
   13e12:	c7dd                	beqz	a5,13ec0 <__addtf3+0x9da>
   13e14:	00df9793          	slli	a5,t6,0xd
   13e18:	8e7e                	mv	t3,t6
   13e1a:	85f6                	mv	a1,t4
   13e1c:	8a42                	mv	s4,a6
   13e1e:	0007ce63          	bltz	a5,13e3a <__addtf3+0x954>
   13e22:	48c1                	li	a7,16
   13e24:	b2ed                	j	1380e <__addtf3+0x328>
   13e26:	01dfe7b3          	or	a5,t6,t4
   13e2a:	c20786e3          	beqz	a5,13a56 <__addtf3+0x570>
   13e2e:	00df9793          	slli	a5,t6,0xd
   13e32:	8e7e                	mv	t3,t6
   13e34:	85f6                	mv	a1,t4
   13e36:	fe07d6e3          	bgez	a5,13e22 <__addtf3+0x93c>
   13e3a:	4881                	li	a7,0
   13e3c:	bac9                	j	1380e <__addtf3+0x328>
   13e3e:	01dfeeb3          	or	t4,t6,t4
   13e42:	040e9563          	bnez	t4,13e8c <__addtf3+0x9a6>
   13e46:	d4071ee3          	bnez	a4,13ba2 <__addtf3+0x6bc>
   13e4a:	00361793          	slli	a5,a2,0x3
   13e4e:	838d                	srli	a5,a5,0x3
   13e50:	03df9513          	slli	a0,t6,0x3d
   13e54:	8d5d                	or	a0,a0,a5
   13e56:	003fd793          	srli	a5,t6,0x3
   13e5a:	bad1                	j	1382e <__addtf3+0x348>
   13e5c:	00e667b3          	or	a5,a2,a4
   13e60:	df9d                	beqz	a5,13d9e <__addtf3+0x8b8>
   13e62:	85b2                	mv	a1,a2
   13e64:	00767793          	andi	a5,a2,7
   13e68:	8e3a                	mv	t3,a4
   13e6a:	892e                	mv	s2,a1
   13e6c:	faeff06f          	j	1361a <__addtf3+0x134>
   13e70:	cb31                	beqz	a4,13ec4 <__addtf3+0x9de>
   13e72:	00de1793          	slli	a5,t3,0xd
   13e76:	0007c363          	bltz	a5,13e7c <__addtf3+0x996>
   13e7a:	48c1                	li	a7,16
   13e7c:	67a1                	lui	a5,0x8
   13e7e:	17fd                	addi	a5,a5,-1
   13e80:	ecf319e3          	bne	t1,a5,13d52 <__addtf3+0x86c>
   13e84:	01dfe5b3          	or	a1,t6,t4
   13e88:	d0058de3          	beqz	a1,13ba2 <__addtf3+0x6bc>
   13e8c:	00df9793          	slli	a5,t6,0xd
   13e90:	0007c363          	bltz	a5,13e96 <__addtf3+0x9b0>
   13e94:	48c1                	li	a7,16
   13e96:	db55                	beqz	a4,13e4a <__addtf3+0x964>
   13e98:	b5c9                	j	13d5a <__addtf3+0x874>
   13e9a:	e60a0be3          	beqz	s4,13d10 <__addtf3+0x82a>
   13e9e:	ffe78413          	addi	s0,a5,-2 # 7ffe <exit-0x8122>
   13ea2:	4581                	li	a1,0
   13ea4:	4895                	li	a7,5
   13ea6:	5e7d                	li	t3,-1
   13ea8:	597d                	li	s2,-1
   13eaa:	fbcff06f          	j	13666 <__addtf3+0x180>
   13eae:	01d58933          	add	s2,a1,t4
   13eb2:	01fe0733          	add	a4,t3,t6
   13eb6:	01d93e33          	sltu	t3,s2,t4
   13eba:	9e3a                	add	t3,t3,a4
   13ebc:	841a                	mv	s0,t1
   13ebe:	be45                	j	13a6e <__addtf3+0x588>
   13ec0:	8a42                	mv	s4,a6
   13ec2:	be51                	j	13a56 <__addtf3+0x570>
   13ec4:	f88313e3          	bne	t1,s0,13e4a <__addtf3+0x964>
   13ec8:	01dfeeb3          	or	t4,t6,t4
   13ecc:	fc0e90e3          	bnez	t4,13e8c <__addtf3+0x9a6>
   13ed0:	bfad                	j	13e4a <__addtf3+0x964>
   13ed2:	e20a0fe3          	beqz	s4,13d10 <__addtf3+0x82a>
   13ed6:	4581                	li	a1,0
   13ed8:	847a                	mv	s0,t5
   13eda:	4895                	li	a7,5
   13edc:	5e7d                	li	t3,-1
   13ede:	597d                	li	s2,-1
   13ee0:	f86ff06f          	j	13666 <__addtf3+0x180>

0000000000013ee4 <__eqtf2>:
   13ee4:	832a                	mv	t1,a0
   13ee6:	002027f3          	frrm	a5
   13eea:	6721                	lui	a4,0x8
   13eec:	177d                	addi	a4,a4,-1
   13eee:	0305d813          	srli	a6,a1,0x30
   13ef2:	57fd                	li	a5,-1
   13ef4:	83c1                	srli	a5,a5,0x10
   13ef6:	0306d893          	srli	a7,a3,0x30
   13efa:	00e87833          	and	a6,a6,a4
   13efe:	00f5fe33          	and	t3,a1,a5
   13f02:	00e8f8b3          	and	a7,a7,a4
   13f06:	8ff5                	and	a5,a5,a3
   13f08:	91fd                	srli	a1,a1,0x3f
   13f0a:	92fd                	srli	a3,a3,0x3f
   13f0c:	00e80c63          	beq	a6,a4,13f24 <__eqtf2+0x40>
   13f10:	00e88663          	beq	a7,a4,13f1c <__eqtf2+0x38>
   13f14:	4505                	li	a0,1
   13f16:	03180063          	beq	a6,a7,13f36 <__eqtf2+0x52>
   13f1a:	8082                	ret
   13f1c:	8e5d                	or	a2,a2,a5
   13f1e:	ea29                	bnez	a2,13f70 <__eqtf2+0x8c>
   13f20:	4505                	li	a0,1
   13f22:	8082                	ret
   13f24:	00ae6733          	or	a4,t3,a0
   13f28:	e71d                	bnez	a4,13f56 <__eqtf2+0x72>
   13f2a:	4505                	li	a0,1
   13f2c:	ff0897e3          	bne	a7,a6,13f1a <__eqtf2+0x36>
   13f30:	00c7e733          	or	a4,a5,a2
   13f34:	ef15                	bnez	a4,13f70 <__eqtf2+0x8c>
   13f36:	4505                	li	a0,1
   13f38:	fefe11e3          	bne	t3,a5,13f1a <__eqtf2+0x36>
   13f3c:	fcc31fe3          	bne	t1,a2,13f1a <__eqtf2+0x36>
   13f40:	4501                	li	a0,0
   13f42:	fcd58ce3          	beq	a1,a3,13f1a <__eqtf2+0x36>
   13f46:	4505                	li	a0,1
   13f48:	fc0819e3          	bnez	a6,13f1a <__eqtf2+0x36>
   13f4c:	006e6533          	or	a0,t3,t1
   13f50:	00a03533          	snez	a0,a0
   13f54:	8082                	ret
   13f56:	02fe5e13          	srli	t3,t3,0x2f
   13f5a:	000e1663          	bnez	t3,13f66 <__eqtf2+0x82>
   13f5e:	00186073          	csrsi	fflags,16
   13f62:	4505                	li	a0,1
   13f64:	8082                	ret
   13f66:	4505                	li	a0,1
   13f68:	fb0899e3          	bne	a7,a6,13f1a <__eqtf2+0x36>
   13f6c:	8e5d                	or	a2,a2,a5
   13f6e:	d655                	beqz	a2,13f1a <__eqtf2+0x36>
   13f70:	93bd                	srli	a5,a5,0x2f
   13f72:	d7f5                	beqz	a5,13f5e <__eqtf2+0x7a>
   13f74:	b775                	j	13f20 <__eqtf2+0x3c>

0000000000013f76 <__multf3>:
   13f76:	7159                	addi	sp,sp,-112
   13f78:	e8ca                	sd	s2,80(sp)
   13f7a:	e0d2                	sd	s4,64(sp)
   13f7c:	ec66                	sd	s9,24(sp)
   13f7e:	f486                	sd	ra,104(sp)
   13f80:	f0a2                	sd	s0,96(sp)
   13f82:	eca6                	sd	s1,88(sp)
   13f84:	e4ce                	sd	s3,72(sp)
   13f86:	fc56                	sd	s5,56(sp)
   13f88:	f85a                	sd	s6,48(sp)
   13f8a:	f45e                	sd	s7,40(sp)
   13f8c:	f062                	sd	s8,32(sp)
   13f8e:	e86a                	sd	s10,16(sp)
   13f90:	e46e                	sd	s11,8(sp)
   13f92:	892a                	mv	s2,a0
   13f94:	8cb2                	mv	s9,a2
   13f96:	8a36                	mv	s4,a3
   13f98:	00202b73          	frrm	s6
   13f9c:	6721                	lui	a4,0x8
   13f9e:	0305d793          	srli	a5,a1,0x30
   13fa2:	177d                	addi	a4,a4,-1
   13fa4:	01059413          	slli	s0,a1,0x10
   13fa8:	8ff9                	and	a5,a5,a4
   13faa:	2b01                	sext.w	s6,s6
   13fac:	8041                	srli	s0,s0,0x10
   13fae:	03f5db93          	srli	s7,a1,0x3f
   13fb2:	cffd                	beqz	a5,140b0 <__multf3+0x13a>
   13fb4:	00078a9b          	sext.w	s5,a5
   13fb8:	12e78d63          	beq	a5,a4,140f2 <__multf3+0x17c>
   13fbc:	040e                	slli	s0,s0,0x3
   13fbe:	03d55713          	srli	a4,a0,0x3d
   13fc2:	8f41                	or	a4,a4,s0
   13fc4:	7af1                	lui	s5,0xffffc
   13fc6:	4405                	li	s0,1
   13fc8:	144e                	slli	s0,s0,0x33
   13fca:	0a85                	addi	s5,s5,1
   13fcc:	8c59                	or	s0,s0,a4
   13fce:	00351493          	slli	s1,a0,0x3
   13fd2:	9abe                	add	s5,s5,a5
   13fd4:	4c01                	li	s8,0
   13fd6:	4d01                	li	s10,0
   13fd8:	4981                	li	s3,0
   13fda:	66a1                	lui	a3,0x8
   13fdc:	030a5713          	srli	a4,s4,0x30
   13fe0:	16fd                	addi	a3,a3,-1
   13fe2:	010a1913          	slli	s2,s4,0x10
   13fe6:	8f75                	and	a4,a4,a3
   13fe8:	8de6                	mv	s11,s9
   13fea:	01095913          	srli	s2,s2,0x10
   13fee:	03fa5a13          	srli	s4,s4,0x3f
   13ff2:	10070e63          	beqz	a4,1410e <__multf3+0x198>
   13ff6:	0007061b          	sext.w	a2,a4
   13ffa:	04d70463          	beq	a4,a3,14042 <__multf3+0xcc>
   13ffe:	090e                	slli	s2,s2,0x3
   14000:	03dcd793          	srli	a5,s9,0x3d
   14004:	76f1                	lui	a3,0xffffc
   14006:	0127e7b3          	or	a5,a5,s2
   1400a:	0685                	addi	a3,a3,1
   1400c:	4905                	li	s2,1
   1400e:	9736                	add	a4,a4,a3
   14010:	194e                	slli	s2,s2,0x33
   14012:	0127e933          	or	s2,a5,s2
   14016:	003c9d93          	slli	s11,s9,0x3
   1401a:	9aba                	add	s5,s5,a4
   1401c:	4601                	li	a2,0
   1401e:	014bc733          	xor	a4,s7,s4
   14022:	46bd                	li	a3,15
   14024:	883a                	mv	a6,a4
   14026:	001a8593          	addi	a1,s5,1 # ffffffffffffc001 <__global_pointer$+0xfffffffffffe3801>
   1402a:	1386e263          	bltu	a3,s8,1414e <__multf3+0x1d8>
   1402e:	00002697          	auipc	a3,0x2
   14032:	21268693          	addi	a3,a3,530 # 16240 <__func__.2721+0x10>
   14036:	002c1793          	slli	a5,s8,0x2
   1403a:	97b6                	add	a5,a5,a3
   1403c:	439c                	lw	a5,0(a5)
   1403e:	97b6                	add	a5,a5,a3
   14040:	8782                	jr	a5
   14042:	019967b3          	or	a5,s2,s9
   14046:	9ab2                	add	s5,s5,a2
   14048:	32078163          	beqz	a5,1436a <__multf3+0x3f4>
   1404c:	01091793          	slli	a5,s2,0x10
   14050:	003c6c13          	ori	s8,s8,3
   14054:	460d                	li	a2,3
   14056:	fc07c4e3          	bltz	a5,1401e <__multf3+0xa8>
   1405a:	49c1                	li	s3,16
   1405c:	b7c9                	j	1401e <__multf3+0xa8>
   1405e:	8852                	mv	a6,s4
   14060:	844a                	mv	s0,s2
   14062:	84ee                	mv	s1,s11
   14064:	8d32                	mv	s10,a2
   14066:	4789                	li	a5,2
   14068:	2efd0963          	beq	s10,a5,1435a <__multf3+0x3e4>
   1406c:	478d                	li	a5,3
   1406e:	3efd0363          	beq	s10,a5,14454 <__multf3+0x4de>
   14072:	4785                	li	a5,1
   14074:	8742                	mv	a4,a6
   14076:	28fd1263          	bne	s10,a5,142fa <__multf3+0x384>
   1407a:	4681                	li	a3,0
   1407c:	4781                	li	a5,0
   1407e:	4501                	li	a0,0
   14080:	073e                	slli	a4,a4,0xf
   14082:	01079593          	slli	a1,a5,0x10
   14086:	8f55                	or	a4,a4,a3
   14088:	1742                	slli	a4,a4,0x30
   1408a:	81c1                	srli	a1,a1,0x10
   1408c:	8dd9                	or	a1,a1,a4
   1408e:	3c099063          	bnez	s3,1444e <__multf3+0x4d8>
   14092:	70a6                	ld	ra,104(sp)
   14094:	7406                	ld	s0,96(sp)
   14096:	64e6                	ld	s1,88(sp)
   14098:	6946                	ld	s2,80(sp)
   1409a:	69a6                	ld	s3,72(sp)
   1409c:	6a06                	ld	s4,64(sp)
   1409e:	7ae2                	ld	s5,56(sp)
   140a0:	7b42                	ld	s6,48(sp)
   140a2:	7ba2                	ld	s7,40(sp)
   140a4:	7c02                	ld	s8,32(sp)
   140a6:	6ce2                	ld	s9,24(sp)
   140a8:	6d42                	ld	s10,16(sp)
   140aa:	6da2                	ld	s11,8(sp)
   140ac:	6165                	addi	sp,sp,112
   140ae:	8082                	ret
   140b0:	00a464b3          	or	s1,s0,a0
   140b4:	2c048c63          	beqz	s1,1438c <__multf3+0x416>
   140b8:	3c040a63          	beqz	s0,1448c <__multf3+0x516>
   140bc:	8522                	mv	a0,s0
   140be:	2a4010ef          	jal	ra,15362 <__clzdi2>
   140c2:	0005071b          	sext.w	a4,a0
   140c6:	3545                	addiw	a0,a0,-15
   140c8:	03d00793          	li	a5,61
   140cc:	0035049b          	addiw	s1,a0,3
   140d0:	40a7853b          	subw	a0,a5,a0
   140d4:	00941433          	sll	s0,s0,s1
   140d8:	00a95533          	srl	a0,s2,a0
   140dc:	8c49                	or	s0,s0,a0
   140de:	009914b3          	sll	s1,s2,s1
   140e2:	77f1                	lui	a5,0xffffc
   140e4:	07c5                	addi	a5,a5,17
   140e6:	40e78ab3          	sub	s5,a5,a4
   140ea:	4c01                	li	s8,0
   140ec:	4d01                	li	s10,0
   140ee:	4981                	li	s3,0
   140f0:	b5ed                	j	13fda <__multf3+0x64>
   140f2:	00a464b3          	or	s1,s0,a0
   140f6:	28048663          	beqz	s1,14382 <__multf3+0x40c>
   140fa:	01041793          	slli	a5,s0,0x10
   140fe:	84aa                	mv	s1,a0
   14100:	4c31                	li	s8,12
   14102:	4d0d                	li	s10,3
   14104:	49c1                	li	s3,16
   14106:	ec07dae3          	bgez	a5,13fda <__multf3+0x64>
   1410a:	4981                	li	s3,0
   1410c:	b5f9                	j	13fda <__multf3+0x64>
   1410e:	019967b3          	or	a5,s2,s9
   14112:	26078263          	beqz	a5,14376 <__multf3+0x400>
   14116:	34090663          	beqz	s2,14462 <__multf3+0x4ec>
   1411a:	854a                	mv	a0,s2
   1411c:	246010ef          	jal	ra,15362 <__clzdi2>
   14120:	0005071b          	sext.w	a4,a0
   14124:	3545                	addiw	a0,a0,-15
   14126:	03d00793          	li	a5,61
   1412a:	00350d9b          	addiw	s11,a0,3
   1412e:	40a7853b          	subw	a0,a5,a0
   14132:	01b91933          	sll	s2,s2,s11
   14136:	00acd533          	srl	a0,s9,a0
   1413a:	01256933          	or	s2,a0,s2
   1413e:	01bc9db3          	sll	s11,s9,s11
   14142:	77f1                	lui	a5,0xffffc
   14144:	07c5                	addi	a5,a5,17
   14146:	8f99                	sub	a5,a5,a4
   14148:	9abe                	add	s5,s5,a5
   1414a:	4601                	li	a2,0
   1414c:	bdc9                	j	1401e <__multf3+0xa8>
   1414e:	57fd                	li	a5,-1
   14150:	9381                	srli	a5,a5,0x20
   14152:	020dd613          	srli	a2,s11,0x20
   14156:	00f4f6b3          	and	a3,s1,a5
   1415a:	0204dc13          	srli	s8,s1,0x20
   1415e:	00fdf7b3          	and	a5,s11,a5
   14162:	02d78333          	mul	t1,a5,a3
   14166:	012d82b3          	add	t0,s11,s2
   1416a:	009403b3          	add	t2,s0,s1
   1416e:	0093ba33          	sltu	s4,t2,s1
   14172:	01b2bbb3          	sltu	s7,t0,s11
   14176:	8ed2                	mv	t4,s4
   14178:	8dde                	mv	s11,s7
   1417a:	02fc07b3          	mul	a5,s8,a5
   1417e:	02035513          	srli	a0,t1,0x20
   14182:	02d606b3          	mul	a3,a2,a3
   14186:	96be                	add	a3,a3,a5
   14188:	9536                	add	a0,a0,a3
   1418a:	02cc0c33          	mul	s8,s8,a2
   1418e:	00f57563          	bgeu	a0,a5,14198 <__multf3+0x222>
   14192:	4785                	li	a5,1
   14194:	1782                	slli	a5,a5,0x20
   14196:	9c3e                	add	s8,s8,a5
   14198:	567d                	li	a2,-1
   1419a:	9201                	srli	a2,a2,0x20
   1419c:	00c3f7b3          	and	a5,t2,a2
   141a0:	0203df13          	srli	t5,t2,0x20
   141a4:	0202d893          	srli	a7,t0,0x20
   141a8:	00c2ffb3          	and	t6,t0,a2
   141ac:	00c574b3          	and	s1,a0,a2
   141b0:	02ff8d33          	mul	s10,t6,a5
   141b4:	00c37333          	and	t1,t1,a2
   141b8:	9101                	srli	a0,a0,0x20
   141ba:	1482                	slli	s1,s1,0x20
   141bc:	9562                	add	a0,a0,s8
   141be:	9326                	add	t1,t1,s1
   141c0:	03ff0633          	mul	a2,t5,t6
   141c4:	020d5693          	srli	a3,s10,0x20
   141c8:	02f887b3          	mul	a5,a7,a5
   141cc:	97b2                	add	a5,a5,a2
   141ce:	97b6                	add	a5,a5,a3
   141d0:	031f0f33          	mul	t5,t5,a7
   141d4:	00c7f563          	bgeu	a5,a2,141de <__multf3+0x268>
   141d8:	4685                	li	a3,1
   141da:	1682                	slli	a3,a3,0x20
   141dc:	9f36                	add	t5,t5,a3
   141de:	5ffd                	li	t6,-1
   141e0:	020fdf93          	srli	t6,t6,0x20
   141e4:	02045c93          	srli	s9,s0,0x20
   141e8:	02095613          	srli	a2,s2,0x20
   141ec:	01f47433          	and	s0,s0,t6
   141f0:	01f97933          	and	s2,s2,t6
   141f4:	03240c33          	mul	s8,s0,s2
   141f8:	01f7fe33          	and	t3,a5,t6
   141fc:	1e02                	slli	t3,t3,0x20
   141fe:	01fd7fb3          	and	t6,s10,t6
   14202:	9381                	srli	a5,a5,0x20
   14204:	9e7e                	add	t3,t3,t6
   14206:	032c8933          	mul	s2,s9,s2
   1420a:	020c5893          	srli	a7,s8,0x20
   1420e:	02860433          	mul	s0,a2,s0
   14212:	944a                	add	s0,s0,s2
   14214:	98a2                	add	a7,a7,s0
   14216:	02cc8cb3          	mul	s9,s9,a2
   1421a:	0128f563          	bgeu	a7,s2,14224 <__multf3+0x2ae>
   1421e:	4685                	li	a3,1
   14220:	1682                	slli	a3,a3,0x20
   14222:	9cb6                	add	s9,s9,a3
   14224:	9e2a                	add	t3,t3,a0
   14226:	00ae3633          	sltu	a2,t3,a0
   1422a:	97b2                	add	a5,a5,a2
   1422c:	41b00db3          	neg	s11,s11
   14230:	01e786b3          	add	a3,a5,t5
   14234:	41d004b3          	neg	s1,t4
   14238:	007df3b3          	and	t2,s11,t2
   1423c:	597d                	li	s2,-1
   1423e:	93b6                	add	t2,t2,a3
   14240:	02095913          	srli	s2,s2,0x20
   14244:	00c6b633          	sltu	a2,a3,a2
   14248:	0054f2b3          	and	t0,s1,t0
   1424c:	017a7a33          	and	s4,s4,s7
   14250:	929e                	add	t0,t0,t2
   14252:	0128f7b3          	and	a5,a7,s2
   14256:	9652                	add	a2,a2,s4
   14258:	00d3b6b3          	sltu	a3,t2,a3
   1425c:	406e0fb3          	sub	t6,t3,t1
   14260:	40a28f33          	sub	t5,t0,a0
   14264:	96b2                	add	a3,a3,a2
   14266:	0208d893          	srli	a7,a7,0x20
   1426a:	1782                	slli	a5,a5,0x20
   1426c:	012c7c33          	and	s8,s8,s2
   14270:	0072b3b3          	sltu	t2,t0,t2
   14274:	01fe3633          	sltu	a2,t3,t6
   14278:	98e6                	add	a7,a7,s9
   1427a:	97e2                	add	a5,a5,s8
   1427c:	969e                	add	a3,a3,t2
   1427e:	01e2beb3          	sltu	t4,t0,t5
   14282:	40cf0633          	sub	a2,t5,a2
   14286:	01fe7563          	bgeu	t3,t6,14290 <__multf3+0x31a>
   1428a:	00551363          	bne	a0,t0,14290 <__multf3+0x31a>
   1428e:	4e85                	li	t4,1
   14290:	40ff8e33          	sub	t3,t6,a5
   14294:	41160533          	sub	a0,a2,a7
   14298:	01cfb4b3          	sltu	s1,t6,t3
   1429c:	96c6                	add	a3,a3,a7
   1429e:	00a63633          	sltu	a2,a2,a0
   142a2:	409784b3          	sub	s1,a5,s1
   142a6:	01cff863          	bgeu	t6,t3,142b6 <__multf3+0x340>
   142aa:	00153893          	seqz	a7,a0
   142ae:	01166633          	or	a2,a2,a7
   142b2:	0ff67613          	zext.b	a2,a2
   142b6:	9526                	add	a0,a0,s1
   142b8:	00f537b3          	sltu	a5,a0,a5
   142bc:	97b6                	add	a5,a5,a3
   142be:	00de1493          	slli	s1,t3,0xd
   142c2:	41d787b3          	sub	a5,a5,t4
   142c6:	0064e4b3          	or	s1,s1,t1
   142ca:	8f91                	sub	a5,a5,a2
   142cc:	033e5693          	srli	a3,t3,0x33
   142d0:	07b6                	slli	a5,a5,0xd
   142d2:	009034b3          	snez	s1,s1
   142d6:	03355413          	srli	s0,a0,0x33
   142da:	8cd5                	or	s1,s1,a3
   142dc:	0536                	slli	a0,a0,0xd
   142de:	00b79693          	slli	a3,a5,0xb
   142e2:	8c5d                	or	s0,s0,a5
   142e4:	8cc9                	or	s1,s1,a0
   142e6:	2206d863          	bgez	a3,14516 <__multf3+0x5a0>
   142ea:	0014d793          	srli	a5,s1,0x1
   142ee:	8885                	andi	s1,s1,1
   142f0:	03f41693          	slli	a3,s0,0x3f
   142f4:	8cdd                	or	s1,s1,a5
   142f6:	8cd5                	or	s1,s1,a3
   142f8:	8005                	srli	s0,s0,0x1
   142fa:	6691                	lui	a3,0x4
   142fc:	16fd                	addi	a3,a3,-1
   142fe:	00d58633          	add	a2,a1,a3
   14302:	08c05b63          	blez	a2,14398 <__multf3+0x422>
   14306:	0074f793          	andi	a5,s1,7
   1430a:	cb99                	beqz	a5,14320 <__multf3+0x3aa>
   1430c:	4789                	li	a5,2
   1430e:	0019e993          	ori	s3,s3,1
   14312:	28fb0b63          	beq	s6,a5,145a8 <__multf3+0x632>
   14316:	478d                	li	a5,3
   14318:	24fb0163          	beq	s6,a5,1455a <__multf3+0x5e4>
   1431c:	220b0363          	beqz	s6,14542 <__multf3+0x5cc>
   14320:	00b41793          	slli	a5,s0,0xb
   14324:	0007d963          	bgez	a5,14336 <__multf3+0x3c0>
   14328:	57fd                	li	a5,-1
   1432a:	17d2                	slli	a5,a5,0x34
   1432c:	17fd                	addi	a5,a5,-1
   1432e:	6691                	lui	a3,0x4
   14330:	8c7d                	and	s0,s0,a5
   14332:	00d58633          	add	a2,a1,a3
   14336:	67a1                	lui	a5,0x8
   14338:	ffe78593          	addi	a1,a5,-2 # 7ffe <exit-0x8122>
   1433c:	1ac5ce63          	blt	a1,a2,144f8 <__multf3+0x582>
   14340:	fff78693          	addi	a3,a5,-1
   14344:	8ef1                	and	a3,a3,a2
   14346:	808d                	srli	s1,s1,0x3
   14348:	03d41513          	slli	a0,s0,0x3d
   1434c:	00d41793          	slli	a5,s0,0xd
   14350:	16c2                	slli	a3,a3,0x30
   14352:	8d45                	or	a0,a0,s1
   14354:	83c1                	srli	a5,a5,0x10
   14356:	92c1                	srli	a3,a3,0x30
   14358:	b325                	j	14080 <__multf3+0x10a>
   1435a:	66a1                	lui	a3,0x8
   1435c:	8742                	mv	a4,a6
   1435e:	16fd                	addi	a3,a3,-1
   14360:	4781                	li	a5,0
   14362:	4501                	li	a0,0
   14364:	bb31                	j	14080 <__multf3+0x10a>
   14366:	885e                	mv	a6,s7
   14368:	b9fd                	j	14066 <__multf3+0xf0>
   1436a:	002c6c13          	ori	s8,s8,2
   1436e:	4901                	li	s2,0
   14370:	4d81                	li	s11,0
   14372:	4609                	li	a2,2
   14374:	b16d                	j	1401e <__multf3+0xa8>
   14376:	001c6c13          	ori	s8,s8,1
   1437a:	4901                	li	s2,0
   1437c:	4d81                	li	s11,0
   1437e:	4605                	li	a2,1
   14380:	b979                	j	1401e <__multf3+0xa8>
   14382:	4401                	li	s0,0
   14384:	4c21                	li	s8,8
   14386:	4d09                	li	s10,2
   14388:	4981                	li	s3,0
   1438a:	b981                	j	13fda <__multf3+0x64>
   1438c:	4401                	li	s0,0
   1438e:	4c11                	li	s8,4
   14390:	4a81                	li	s5,0
   14392:	4d05                	li	s10,1
   14394:	4981                	li	s3,0
   14396:	b191                	j	13fda <__multf3+0x64>
   14398:	4785                	li	a5,1
   1439a:	40c785b3          	sub	a1,a5,a2
   1439e:	10061b63          	bnez	a2,144b4 <__multf3+0x53e>
   143a2:	0074f693          	andi	a3,s1,7
   143a6:	87a2                	mv	a5,s0
   143a8:	ca85                	beqz	a3,143d8 <__multf3+0x462>
   143aa:	4789                	li	a5,2
   143ac:	0019e993          	ori	s3,s3,1
   143b0:	24fb0463          	beq	s6,a5,145f8 <__multf3+0x682>
   143b4:	478d                	li	a5,3
   143b6:	22fb0663          	beq	s6,a5,145e2 <__multf3+0x66c>
   143ba:	87a2                	mv	a5,s0
   143bc:	000b1e63          	bnez	s6,143d8 <__multf3+0x462>
   143c0:	00f4f693          	andi	a3,s1,15
   143c4:	4611                	li	a2,4
   143c6:	00c68963          	beq	a3,a2,143d8 <__multf3+0x462>
   143ca:	ffc4b793          	sltiu	a5,s1,-4
   143ce:	0017c793          	xori	a5,a5,1
   143d2:	1782                	slli	a5,a5,0x20
   143d4:	9381                	srli	a5,a5,0x20
   143d6:	97a2                	add	a5,a5,s0
   143d8:	93d1                	srli	a5,a5,0x34
   143da:	0017c793          	xori	a5,a5,1
   143de:	0017f893          	andi	a7,a5,1
   143e2:	0005869b          	sext.w	a3,a1
   143e6:	04000613          	li	a2,64
   143ea:	9e15                	subw	a2,a2,a3
   143ec:	00d4d7b3          	srl	a5,s1,a3
   143f0:	00c41533          	sll	a0,s0,a2
   143f4:	00c494b3          	sll	s1,s1,a2
   143f8:	8d5d                	or	a0,a0,a5
   143fa:	009034b3          	snez	s1,s1
   143fe:	8d45                	or	a0,a0,s1
   14400:	00757793          	andi	a5,a0,7
   14404:	00d45433          	srl	s0,s0,a3
   14408:	c795                	beqz	a5,14434 <__multf3+0x4be>
   1440a:	4789                	li	a5,2
   1440c:	0019e993          	ori	s3,s3,1
   14410:	20fb0163          	beq	s6,a5,14612 <__multf3+0x69c>
   14414:	478d                	li	a5,3
   14416:	1efb0563          	beq	s6,a5,14600 <__multf3+0x68a>
   1441a:	000b1d63          	bnez	s6,14434 <__multf3+0x4be>
   1441e:	00f57793          	andi	a5,a0,15
   14422:	4691                	li	a3,4
   14424:	00d78863          	beq	a5,a3,14434 <__multf3+0x4be>
   14428:	00450793          	addi	a5,a0,4
   1442c:	00a7b533          	sltu	a0,a5,a0
   14430:	942a                	add	s0,s0,a0
   14432:	853e                	mv	a0,a5
   14434:	00c41793          	slli	a5,s0,0xc
   14438:	1807db63          	bgez	a5,145ce <__multf3+0x658>
   1443c:	0019e993          	ori	s3,s3,1
   14440:	18089163          	bnez	a7,145c2 <__multf3+0x64c>
   14444:	4585                	li	a1,1
   14446:	177e                	slli	a4,a4,0x3f
   14448:	15c2                	slli	a1,a1,0x30
   1444a:	4501                	li	a0,0
   1444c:	8dd9                	or	a1,a1,a4
   1444e:	0019a073          	csrs	fflags,s3
   14452:	b181                	j	14092 <__multf3+0x11c>
   14454:	66a1                	lui	a3,0x8
   14456:	4785                	li	a5,1
   14458:	4701                	li	a4,0
   1445a:	16fd                	addi	a3,a3,-1
   1445c:	17be                	slli	a5,a5,0x2f
   1445e:	4501                	li	a0,0
   14460:	b105                	j	14080 <__multf3+0x10a>
   14462:	8566                	mv	a0,s9
   14464:	6ff000ef          	jal	ra,15362 <__clzdi2>
   14468:	0005079b          	sext.w	a5,a0
   1446c:	03178693          	addi	a3,a5,49
   14470:	03c00613          	li	a2,60
   14474:	04078713          	addi	a4,a5,64
   14478:	0006851b          	sext.w	a0,a3
   1447c:	cad655e3          	bge	a2,a3,14126 <__multf3+0x1b0>
   14480:	fc35051b          	addiw	a0,a0,-61
   14484:	00ac9933          	sll	s2,s9,a0
   14488:	4d81                	li	s11,0
   1448a:	b965                	j	14142 <__multf3+0x1cc>
   1448c:	6d7000ef          	jal	ra,15362 <__clzdi2>
   14490:	00050a9b          	sext.w	s5,a0
   14494:	031a8793          	addi	a5,s5,49
   14498:	03c00693          	li	a3,60
   1449c:	040a8713          	addi	a4,s5,64
   144a0:	0007851b          	sext.w	a0,a5
   144a4:	c2f6d2e3          	bge	a3,a5,140c8 <__multf3+0x152>
   144a8:	fc35051b          	addiw	a0,a0,-61
   144ac:	00a91433          	sll	s0,s2,a0
   144b0:	4481                	li	s1,0
   144b2:	b905                	j	140e2 <__multf3+0x16c>
   144b4:	07400693          	li	a3,116
   144b8:	0ab6da63          	bge	a3,a1,1456c <__multf3+0x5f6>
   144bc:	00946533          	or	a0,s0,s1
   144c0:	cd09                	beqz	a0,144da <__multf3+0x564>
   144c2:	4689                	li	a3,2
   144c4:	0019e993          	ori	s3,s3,1
   144c8:	8542                	mv	a0,a6
   144ca:	00db0863          	beq	s6,a3,144da <__multf3+0x564>
   144ce:	468d                	li	a3,3
   144d0:	4501                	li	a0,0
   144d2:	00db1463          	bne	s6,a3,144da <__multf3+0x564>
   144d6:	41078533          	sub	a0,a5,a6
   144da:	0029e993          	ori	s3,s3,2
   144de:	4681                	li	a3,0
   144e0:	4781                	li	a5,0
   144e2:	01079593          	slli	a1,a5,0x10
   144e6:	81c1                	srli	a1,a1,0x10
   144e8:	03069793          	slli	a5,a3,0x30
   144ec:	177e                	slli	a4,a4,0x3f
   144ee:	8ddd                	or	a1,a1,a5
   144f0:	8dd9                	or	a1,a1,a4
   144f2:	0019a073          	csrs	fflags,s3
   144f6:	be71                	j	14092 <__multf3+0x11c>
   144f8:	4689                	li	a3,2
   144fa:	02db0863          	beq	s6,a3,1452a <__multf3+0x5b4>
   144fe:	468d                	li	a3,3
   14500:	02db0e63          	beq	s6,a3,1453c <__multf3+0x5c6>
   14504:	020b0563          	beqz	s6,1452e <__multf3+0x5b8>
   14508:	57fd                	li	a5,-1
   1450a:	86ae                	mv	a3,a1
   1450c:	83c1                	srli	a5,a5,0x10
   1450e:	557d                	li	a0,-1
   14510:	0059e993          	ori	s3,s3,5
   14514:	b7f9                	j	144e2 <__multf3+0x56c>
   14516:	85d6                	mv	a1,s5
   14518:	b3cd                	j	142fa <__multf3+0x384>
   1451a:	0ffff5b7          	lui	a1,0xffff
   1451e:	4501                	li	a0,0
   14520:	158e                	slli	a1,a1,0x23
   14522:	49c1                	li	s3,16
   14524:	0019a073          	csrs	fflags,s3
   14528:	b6ad                	j	14092 <__multf3+0x11c>
   1452a:	fc080fe3          	beqz	a6,14508 <__multf3+0x592>
   1452e:	fff78693          	addi	a3,a5,-1
   14532:	4501                	li	a0,0
   14534:	4781                	li	a5,0
   14536:	0059e993          	ori	s3,s3,5
   1453a:	b765                	j	144e2 <__multf3+0x56c>
   1453c:	fc0816e3          	bnez	a6,14508 <__multf3+0x592>
   14540:	b7fd                	j	1452e <__multf3+0x5b8>
   14542:	00f4f793          	andi	a5,s1,15
   14546:	4691                	li	a3,4
   14548:	dcd78ce3          	beq	a5,a3,14320 <__multf3+0x3aa>
   1454c:	00448793          	addi	a5,s1,4
   14550:	0097b4b3          	sltu	s1,a5,s1
   14554:	9426                	add	s0,s0,s1
   14556:	84be                	mv	s1,a5
   14558:	b3e1                	j	14320 <__multf3+0x3aa>
   1455a:	dc0813e3          	bnez	a6,14320 <__multf3+0x3aa>
   1455e:	00848793          	addi	a5,s1,8
   14562:	0097b4b3          	sltu	s1,a5,s1
   14566:	9426                	add	s0,s0,s1
   14568:	84be                	mv	s1,a5
   1456a:	bb5d                	j	14320 <__multf3+0x3aa>
   1456c:	03f00793          	li	a5,63
   14570:	0005869b          	sext.w	a3,a1
   14574:	0ab7d263          	bge	a5,a1,14618 <__multf3+0x6a2>
   14578:	fc06851b          	addiw	a0,a3,-64
   1457c:	04000613          	li	a2,64
   14580:	00a457b3          	srl	a5,s0,a0
   14584:	00c58963          	beq	a1,a2,14596 <__multf3+0x620>
   14588:	08000613          	li	a2,128
   1458c:	40d606bb          	subw	a3,a2,a3
   14590:	00d41433          	sll	s0,s0,a3
   14594:	8cc1                	or	s1,s1,s0
   14596:	00903533          	snez	a0,s1
   1459a:	8d5d                	or	a0,a0,a5
   1459c:	00757793          	andi	a5,a0,7
   145a0:	c799                	beqz	a5,145ae <__multf3+0x638>
   145a2:	4885                	li	a7,1
   145a4:	4401                	li	s0,0
   145a6:	b595                	j	1440a <__multf3+0x494>
   145a8:	d6080ce3          	beqz	a6,14320 <__multf3+0x3aa>
   145ac:	bf4d                	j	1455e <__multf3+0x5e8>
   145ae:	810d                	srli	a0,a0,0x3
   145b0:	4781                	li	a5,0
   145b2:	0019f613          	andi	a2,s3,1
   145b6:	4681                	li	a3,0
   145b8:	ac0604e3          	beqz	a2,14080 <__multf3+0x10a>
   145bc:	0029e993          	ori	s3,s3,2
   145c0:	b70d                	j	144e2 <__multf3+0x56c>
   145c2:	4685                	li	a3,1
   145c4:	4781                	li	a5,0
   145c6:	4501                	li	a0,0
   145c8:	0029e993          	ori	s3,s3,2
   145cc:	bf19                	j	144e2 <__multf3+0x56c>
   145ce:	00d41793          	slli	a5,s0,0xd
   145d2:	810d                	srli	a0,a0,0x3
   145d4:	1476                	slli	s0,s0,0x3d
   145d6:	83c1                	srli	a5,a5,0x10
   145d8:	8d41                	or	a0,a0,s0
   145da:	fc089ce3          	bnez	a7,145b2 <__multf3+0x63c>
   145de:	4681                	li	a3,0
   145e0:	b445                	j	14080 <__multf3+0x10a>
   145e2:	87a2                	mv	a5,s0
   145e4:	de081ae3          	bnez	a6,143d8 <__multf3+0x462>
   145e8:	ff84b793          	sltiu	a5,s1,-8
   145ec:	0017c793          	xori	a5,a5,1
   145f0:	1782                	slli	a5,a5,0x20
   145f2:	9381                	srli	a5,a5,0x20
   145f4:	97a2                	add	a5,a5,s0
   145f6:	b3cd                	j	143d8 <__multf3+0x462>
   145f8:	87a2                	mv	a5,s0
   145fa:	dc080fe3          	beqz	a6,143d8 <__multf3+0x462>
   145fe:	b7ed                	j	145e8 <__multf3+0x672>
   14600:	e2081ae3          	bnez	a6,14434 <__multf3+0x4be>
   14604:	00850793          	addi	a5,a0,8
   14608:	00a7b533          	sltu	a0,a5,a0
   1460c:	942a                	add	s0,s0,a0
   1460e:	853e                	mv	a0,a5
   14610:	b515                	j	14434 <__multf3+0x4be>
   14612:	e20801e3          	beqz	a6,14434 <__multf3+0x4be>
   14616:	b7fd                	j	14604 <__multf3+0x68e>
   14618:	4885                	li	a7,1
   1461a:	b3f1                	j	143e6 <__multf3+0x470>

000000000001461c <__subtf3>:
   1461c:	7179                	addi	sp,sp,-48
   1461e:	f406                	sd	ra,40(sp)
   14620:	f022                	sd	s0,32(sp)
   14622:	ec26                	sd	s1,24(sp)
   14624:	e84a                	sd	s2,16(sp)
   14626:	e44e                	sd	s3,8(sp)
   14628:	e052                	sd	s4,0(sp)
   1462a:	8faa                	mv	t6,a0
   1462c:	002029f3          	frrm	s3
   14630:	6821                	lui	a6,0x8
   14632:	57fd                	li	a5,-1
   14634:	187d                	addi	a6,a6,-1
   14636:	83c1                	srli	a5,a5,0x10
   14638:	0305d413          	srli	s0,a1,0x30
   1463c:	0306d313          	srli	t1,a3,0x30
   14640:	00f5feb3          	and	t4,a1,a5
   14644:	01047433          	and	s0,s0,a6
   14648:	8ff5                	and	a5,a5,a3
   1464a:	01037333          	and	t1,t1,a6
   1464e:	0e8e                	slli	t4,t4,0x3
   14650:	03d55713          	srli	a4,a0,0x3d
   14654:	03d65893          	srli	a7,a2,0x3d
   14658:	078e                	slli	a5,a5,0x3
   1465a:	40640f3b          	subw	t5,s0,t1
   1465e:	03f5d913          	srli	s2,a1,0x3f
   14662:	2981                	sext.w	s3,s3
   14664:	01d765b3          	or	a1,a4,t4
   14668:	050e                	slli	a0,a0,0x3
   1466a:	00f8eeb3          	or	t4,a7,a5
   1466e:	92fd                	srli	a3,a3,0x3f
   14670:	00361e13          	slli	t3,a2,0x3
   14674:	000f089b          	sext.w	a7,t5
   14678:	1b030563          	beq	t1,a6,14822 <__subtf3+0x206>
   1467c:	0016c693          	xori	a3,a3,1
   14680:	1cd90a63          	beq	s2,a3,14854 <__subtf3+0x238>
   14684:	17105c63          	blez	a7,147fc <__subtf3+0x1e0>
   14688:	1a031563          	bnez	t1,14832 <__subtf3+0x216>
   1468c:	01cee7b3          	or	a5,t4,t3
   14690:	42078063          	beqz	a5,14ab0 <__subtf3+0x494>
   14694:	ffff089b          	addiw	a7,t5,-1
   14698:	6a088e63          	beqz	a7,14d54 <__subtf3+0x738>
   1469c:	67a1                	lui	a5,0x8
   1469e:	17fd                	addi	a5,a5,-1
   146a0:	74f40863          	beq	s0,a5,14df0 <__subtf3+0x7d4>
   146a4:	07400793          	li	a5,116
   146a8:	1b17c163          	blt	a5,a7,1484a <__subtf3+0x22e>
   146ac:	03f00793          	li	a5,63
   146b0:	5717c963          	blt	a5,a7,14c22 <__subtf3+0x606>
   146b4:	04000793          	li	a5,64
   146b8:	411787bb          	subw	a5,a5,a7
   146bc:	011e5733          	srl	a4,t3,a7
   146c0:	00fe94b3          	sll	s1,t4,a5
   146c4:	00fe1e33          	sll	t3,t3,a5
   146c8:	8cd9                	or	s1,s1,a4
   146ca:	01c03e33          	snez	t3,t3
   146ce:	011ed8b3          	srl	a7,t4,a7
   146d2:	01c4e4b3          	or	s1,s1,t3
   146d6:	411585b3          	sub	a1,a1,a7
   146da:	409504b3          	sub	s1,a0,s1
   146de:	00953533          	sltu	a0,a0,s1
   146e2:	8d89                	sub	a1,a1,a0
   146e4:	00c59793          	slli	a5,a1,0xc
   146e8:	2807db63          	bgez	a5,1497e <__subtf3+0x362>
   146ec:	00d59713          	slli	a4,a1,0xd
   146f0:	00d75a13          	srli	s4,a4,0xd
   146f4:	380a0463          	beqz	s4,14a7c <__subtf3+0x460>
   146f8:	8552                	mv	a0,s4
   146fa:	469000ef          	jal	ra,15362 <__clzdi2>
   146fe:	3551                	addiw	a0,a0,-12
   14700:	04000713          	li	a4,64
   14704:	9f09                	subw	a4,a4,a0
   14706:	00aa15b3          	sll	a1,s4,a0
   1470a:	00e4d733          	srl	a4,s1,a4
   1470e:	8f4d                	or	a4,a4,a1
   14710:	00a49a33          	sll	s4,s1,a0
   14714:	34854d63          	blt	a0,s0,14a6e <__subtf3+0x452>
   14718:	9d01                	subw	a0,a0,s0
   1471a:	0015059b          	addiw	a1,a0,1
   1471e:	03f00793          	li	a5,63
   14722:	4cb7ca63          	blt	a5,a1,14bf6 <__subtf3+0x5da>
   14726:	04000793          	li	a5,64
   1472a:	9f8d                	subw	a5,a5,a1
   1472c:	00f714b3          	sll	s1,a4,a5
   14730:	00ba56b3          	srl	a3,s4,a1
   14734:	00fa17b3          	sll	a5,s4,a5
   14738:	8cd5                	or	s1,s1,a3
   1473a:	00f037b3          	snez	a5,a5
   1473e:	8cdd                	or	s1,s1,a5
   14740:	00b755b3          	srl	a1,a4,a1
   14744:	00b4e7b3          	or	a5,s1,a1
   14748:	1a078963          	beqz	a5,148fa <__subtf3+0x2de>
   1474c:	0074f793          	andi	a5,s1,7
   14750:	00149613          	slli	a2,s1,0x1
   14754:	00159713          	slli	a4,a1,0x1
   14758:	03f4d693          	srli	a3,s1,0x3f
   1475c:	00767513          	andi	a0,a2,7
   14760:	96ba                	add	a3,a3,a4
   14762:	4881                	li	a7,0
   14764:	c911                	beqz	a0,14778 <__subtf3+0x15c>
   14766:	4709                	li	a4,2
   14768:	66e98063          	beq	s3,a4,14dc8 <__subtf3+0x7ac>
   1476c:	470d                	li	a4,3
   1476e:	4885                	li	a7,1
   14770:	64e98263          	beq	s3,a4,14db4 <__subtf3+0x798>
   14774:	62098363          	beqz	s3,14d9a <__subtf3+0x77e>
   14778:	4705                	li	a4,1
   1477a:	03471613          	slli	a2,a4,0x34
   1477e:	8ef1                	and	a3,a3,a2
   14780:	0016b613          	seqz	a2,a3
   14784:	4401                	li	s0,0
   14786:	68078763          	beqz	a5,14e14 <__subtf3+0x7f8>
   1478a:	4789                	li	a5,2
   1478c:	2af98863          	beq	s3,a5,14a3c <__subtf3+0x420>
   14790:	478d                	li	a5,3
   14792:	2cf98463          	beq	s3,a5,14a5a <__subtf3+0x43e>
   14796:	26098f63          	beqz	s3,14a14 <__subtf3+0x3f8>
   1479a:	4705                	li	a4,1
   1479c:	174e                	slli	a4,a4,0x33
   1479e:	8f6d                	and	a4,a4,a1
   147a0:	4885                	li	a7,1
   147a2:	c219                	beqz	a2,147a8 <__subtf3+0x18c>
   147a4:	0028e893          	ori	a7,a7,2
   147a8:	18070e63          	beqz	a4,14944 <__subtf3+0x328>
   147ac:	66a1                	lui	a3,0x8
   147ae:	0405                	addi	s0,s0,1
   147b0:	16fd                	addi	a3,a3,-1
   147b2:	1ed40463          	beq	s0,a3,1499a <__subtf3+0x37e>
   147b6:	567d                	li	a2,-1
   147b8:	03361713          	slli	a4,a2,0x33
   147bc:	177d                	addi	a4,a4,-1
   147be:	8f6d                	and	a4,a4,a1
   147c0:	8c75                	and	s0,s0,a3
   147c2:	03d71513          	slli	a0,a4,0x3d
   147c6:	808d                	srli	s1,s1,0x3
   147c8:	830d                	srli	a4,a4,0x3
   147ca:	8241                	srli	a2,a2,0x10
   147cc:	1442                	slli	s0,s0,0x30
   147ce:	8d45                	or	a0,a0,s1
   147d0:	8f71                	and	a4,a4,a2
   147d2:	9041                	srli	s0,s0,0x30
   147d4:	00197693          	andi	a3,s2,1
   147d8:	06be                	slli	a3,a3,0xf
   147da:	01071593          	slli	a1,a4,0x10
   147de:	0086e733          	or	a4,a3,s0
   147e2:	1742                	slli	a4,a4,0x30
   147e4:	81c1                	srli	a1,a1,0x10
   147e6:	8dd9                	or	a1,a1,a4
   147e8:	2e089a63          	bnez	a7,14adc <__subtf3+0x4c0>
   147ec:	70a2                	ld	ra,40(sp)
   147ee:	7402                	ld	s0,32(sp)
   147f0:	64e2                	ld	s1,24(sp)
   147f2:	6942                	ld	s2,16(sp)
   147f4:	69a2                	ld	s3,8(sp)
   147f6:	6a02                	ld	s4,0(sp)
   147f8:	6145                	addi	sp,sp,48
   147fa:	8082                	ret
   147fc:	0a088463          	beqz	a7,148a4 <__subtf3+0x288>
   14800:	32041f63          	bnez	s0,14b3e <__subtf3+0x522>
   14804:	00a5e7b3          	or	a5,a1,a0
   14808:	16078363          	beqz	a5,1496e <__subtf3+0x352>
   1480c:	577d                	li	a4,-1
   1480e:	72e88663          	beq	a7,a4,14f3a <__subtf3+0x91e>
   14812:	67a1                	lui	a5,0x8
   14814:	17fd                	addi	a5,a5,-1
   14816:	6af30b63          	beq	t1,a5,14ecc <__subtf3+0x8b0>
   1481a:	ffff4f13          	not	t5,t5
   1481e:	2f01                	sext.w	t5,t5
   14820:	ae05                	j	14b50 <__subtf3+0x534>
   14822:	01cee7b3          	or	a5,t4,t3
   14826:	e4078be3          	beqz	a5,1467c <__subtf3+0x60>
   1482a:	18d90863          	beq	s2,a3,149ba <__subtf3+0x39e>
   1482e:	fd1057e3          	blez	a7,147fc <__subtf3+0x1e0>
   14832:	67a1                	lui	a5,0x8
   14834:	17fd                	addi	a5,a5,-1
   14836:	52f40863          	beq	s0,a5,14d66 <__subtf3+0x74a>
   1483a:	4785                	li	a5,1
   1483c:	17ce                	slli	a5,a5,0x33
   1483e:	00feeeb3          	or	t4,t4,a5
   14842:	07400793          	li	a5,116
   14846:	e717d3e3          	bge	a5,a7,146ac <__subtf3+0x90>
   1484a:	01cee4b3          	or	s1,t4,t3
   1484e:	009034b3          	snez	s1,s1
   14852:	b561                	j	146da <__subtf3+0xbe>
   14854:	17105363          	blez	a7,149ba <__subtf3+0x39e>
   14858:	0a030c63          	beqz	t1,14910 <__subtf3+0x2f4>
   1485c:	67a1                	lui	a5,0x8
   1485e:	17fd                	addi	a5,a5,-1
   14860:	5cf40663          	beq	s0,a5,14e2c <__subtf3+0x810>
   14864:	4785                	li	a5,1
   14866:	17ce                	slli	a5,a5,0x33
   14868:	00feeeb3          	or	t4,t4,a5
   1486c:	07400793          	li	a5,116
   14870:	3517c263          	blt	a5,a7,14bb4 <__subtf3+0x598>
   14874:	03f00793          	li	a5,63
   14878:	4117d463          	bge	a5,a7,14c80 <__subtf3+0x664>
   1487c:	fc08849b          	addiw	s1,a7,-64
   14880:	04000793          	li	a5,64
   14884:	009ed733          	srl	a4,t4,s1
   14888:	00f88a63          	beq	a7,a5,1489c <__subtf3+0x280>
   1488c:	08000793          	li	a5,128
   14890:	411788bb          	subw	a7,a5,a7
   14894:	011e98b3          	sll	a7,t4,a7
   14898:	011e6e33          	or	t3,t3,a7
   1489c:	01c034b3          	snez	s1,t3
   148a0:	8cd9                	or	s1,s1,a4
   148a2:	ae29                	j	14bbc <__subtf3+0x5a0>
   148a4:	6721                	lui	a4,0x8
   148a6:	00140793          	addi	a5,s0,1
   148aa:	ffe70813          	addi	a6,a4,-2 # 7ffe <exit-0x8122>
   148ae:	0107f7b3          	and	a5,a5,a6
   148b2:	2c079a63          	bnez	a5,14b86 <__subtf3+0x56a>
   148b6:	00a5e833          	or	a6,a1,a0
   148ba:	01cee7b3          	or	a5,t4,t3
   148be:	40041563          	bnez	s0,14cc8 <__subtf3+0x6ac>
   148c2:	4c080463          	beqz	a6,14d8a <__subtf3+0x76e>
   148c6:	4c078863          	beqz	a5,14d96 <__subtf3+0x77a>
   148ca:	41c50633          	sub	a2,a0,t3
   148ce:	00c537b3          	sltu	a5,a0,a2
   148d2:	41d58733          	sub	a4,a1,t4
   148d6:	8f1d                	sub	a4,a4,a5
   148d8:	00c71793          	slli	a5,a4,0xc
   148dc:	6e07d263          	bgez	a5,14fc0 <__subtf3+0x9a4>
   148e0:	40ae04b3          	sub	s1,t3,a0
   148e4:	40be8733          	sub	a4,t4,a1
   148e8:	009e3e33          	sltu	t3,t3,s1
   148ec:	41c705b3          	sub	a1,a4,t3
   148f0:	00b4e7b3          	or	a5,s1,a1
   148f4:	8936                	mv	s2,a3
   148f6:	e4079be3          	bnez	a5,1474c <__subtf3+0x130>
   148fa:	4501                	li	a0,0
   148fc:	4401                	li	s0,0
   148fe:	4881                	li	a7,0
   14900:	01079713          	slli	a4,a5,0x10
   14904:	1446                	slli	s0,s0,0x31
   14906:	8341                	srli	a4,a4,0x10
   14908:	9045                	srli	s0,s0,0x31
   1490a:	00197693          	andi	a3,s2,1
   1490e:	b5e9                	j	147d8 <__subtf3+0x1bc>
   14910:	01cee7b3          	or	a5,t4,t3
   14914:	18078e63          	beqz	a5,14ab0 <__subtf3+0x494>
   14918:	ffff089b          	addiw	a7,t5,-1
   1491c:	3e088263          	beqz	a7,14d00 <__subtf3+0x6e4>
   14920:	67a1                	lui	a5,0x8
   14922:	17fd                	addi	a5,a5,-1
   14924:	f4f414e3          	bne	s0,a5,1486c <__subtf3+0x250>
   14928:	00a5e7b3          	or	a5,a1,a0
   1492c:	42078263          	beqz	a5,14d50 <__subtf3+0x734>
   14930:	0325d893          	srli	a7,a1,0x32
   14934:	0018f893          	andi	a7,a7,1
   14938:	0018b893          	seqz	a7,a7
   1493c:	0892                	slli	a7,a7,0x4
   1493e:	6421                	lui	s0,0x8
   14940:	84aa                	mv	s1,a0
   14942:	147d                	addi	s0,s0,-1
   14944:	6721                	lui	a4,0x8
   14946:	03d59513          	slli	a0,a1,0x3d
   1494a:	808d                	srli	s1,s1,0x3
   1494c:	177d                	addi	a4,a4,-1
   1494e:	8d45                	or	a0,a0,s1
   14950:	0035d793          	srli	a5,a1,0x3
   14954:	fae416e3          	bne	s0,a4,14900 <__subtf3+0x2e4>
   14958:	8d5d                	or	a0,a0,a5
   1495a:	14050363          	beqz	a0,14aa0 <__subtf3+0x484>
   1495e:	6721                	lui	a4,0x8
   14960:	fff70413          	addi	s0,a4,-1 # 7fff <exit-0x8121>
   14964:	4705                	li	a4,1
   14966:	4681                	li	a3,0
   14968:	173e                	slli	a4,a4,0x2f
   1496a:	4501                	li	a0,0
   1496c:	b5b5                	j	147d8 <__subtf3+0x1bc>
   1496e:	67a1                	lui	a5,0x8
   14970:	17fd                	addi	a5,a5,-1
   14972:	5ef30f63          	beq	t1,a5,14f70 <__subtf3+0x954>
   14976:	85f6                	mv	a1,t4
   14978:	84f2                	mv	s1,t3
   1497a:	841a                	mv	s0,t1
   1497c:	8936                	mv	s2,a3
   1497e:	00b4e7b3          	or	a5,s1,a1
   14982:	dc0403e3          	beqz	s0,14748 <__subtf3+0x12c>
   14986:	0074f793          	andi	a5,s1,7
   1498a:	4601                	li	a2,0
   1498c:	de079fe3          	bnez	a5,1478a <__subtf3+0x16e>
   14990:	4705                	li	a4,1
   14992:	174e                	slli	a4,a4,0x33
   14994:	8f6d                	and	a4,a4,a1
   14996:	4881                	li	a7,0
   14998:	bd01                	j	147a8 <__subtf3+0x18c>
   1499a:	10098163          	beqz	s3,14a9c <__subtf3+0x480>
   1499e:	478d                	li	a5,3
   149a0:	0ef98c63          	beq	s3,a5,14a98 <__subtf3+0x47c>
   149a4:	4789                	li	a5,2
   149a6:	36f98463          	beq	s3,a5,14d0e <__subtf3+0x6f2>
   149aa:	557d                	li	a0,-1
   149ac:	6421                	lui	s0,0x8
   149ae:	0058e893          	ori	a7,a7,5
   149b2:	00355793          	srli	a5,a0,0x3
   149b6:	1479                	addi	s0,s0,-2
   149b8:	b7a1                	j	14900 <__subtf3+0x2e4>
   149ba:	12088b63          	beqz	a7,14af0 <__subtf3+0x4d4>
   149be:	2a040263          	beqz	s0,14c62 <__subtf3+0x646>
   149c2:	67a1                	lui	a5,0x8
   149c4:	17fd                	addi	a5,a5,-1
   149c6:	60f30763          	beq	t1,a5,14fd4 <__subtf3+0x9b8>
   149ca:	4785                	li	a5,1
   149cc:	17ce                	slli	a5,a5,0x33
   149ce:	41e00f3b          	negw	t5,t5
   149d2:	8ddd                	or	a1,a1,a5
   149d4:	07400793          	li	a5,116
   149d8:	47e7cc63          	blt	a5,t5,14e50 <__subtf3+0x834>
   149dc:	03f00793          	li	a5,63
   149e0:	53e7ca63          	blt	a5,t5,14f14 <__subtf3+0x8f8>
   149e4:	04000793          	li	a5,64
   149e8:	41e787bb          	subw	a5,a5,t5
   149ec:	00f594b3          	sll	s1,a1,a5
   149f0:	01e55733          	srl	a4,a0,t5
   149f4:	00f517b3          	sll	a5,a0,a5
   149f8:	8cd9                	or	s1,s1,a4
   149fa:	00f037b3          	snez	a5,a5
   149fe:	01e5df33          	srl	t5,a1,t5
   14a02:	8cdd                	or	s1,s1,a5
   14a04:	9efa                	add	t4,t4,t5
   14a06:	94f2                	add	s1,s1,t3
   14a08:	01c4b733          	sltu	a4,s1,t3
   14a0c:	01d705b3          	add	a1,a4,t4
   14a10:	841a                	mv	s0,t1
   14a12:	aa4d                	j	14bc4 <__subtf3+0x5a8>
   14a14:	00f4f793          	andi	a5,s1,15
   14a18:	4711                	li	a4,4
   14a1a:	d8e780e3          	beq	a5,a4,1479a <__subtf3+0x17e>
   14a1e:	ffc4b793          	sltiu	a5,s1,-4
   14a22:	0017c793          	xori	a5,a5,1
   14a26:	1782                	slli	a5,a5,0x20
   14a28:	9381                	srli	a5,a5,0x20
   14a2a:	4705                	li	a4,1
   14a2c:	95be                	add	a1,a1,a5
   14a2e:	174e                	slli	a4,a4,0x33
   14a30:	8f6d                	and	a4,a4,a1
   14a32:	0491                	addi	s1,s1,4
   14a34:	4885                	li	a7,1
   14a36:	d60617e3          	bnez	a2,147a4 <__subtf3+0x188>
   14a3a:	b3bd                	j	147a8 <__subtf3+0x18c>
   14a3c:	d4090fe3          	beqz	s2,1479a <__subtf3+0x17e>
   14a40:	00848793          	addi	a5,s1,8
   14a44:	0097b4b3          	sltu	s1,a5,s1
   14a48:	4705                	li	a4,1
   14a4a:	95a6                	add	a1,a1,s1
   14a4c:	174e                	slli	a4,a4,0x33
   14a4e:	8f6d                	and	a4,a4,a1
   14a50:	84be                	mv	s1,a5
   14a52:	4885                	li	a7,1
   14a54:	d40618e3          	bnez	a2,147a4 <__subtf3+0x188>
   14a58:	bb81                	j	147a8 <__subtf3+0x18c>
   14a5a:	fe0903e3          	beqz	s2,14a40 <__subtf3+0x424>
   14a5e:	4885                	li	a7,1
   14a60:	4705                	li	a4,1
   14a62:	174e                	slli	a4,a4,0x33
   14a64:	8f6d                	and	a4,a4,a1
   14a66:	4905                	li	s2,1
   14a68:	d2061ee3          	bnez	a2,147a4 <__subtf3+0x188>
   14a6c:	bb35                	j	147a8 <__subtf3+0x18c>
   14a6e:	55fd                	li	a1,-1
   14a70:	15ce                	slli	a1,a1,0x33
   14a72:	15fd                	addi	a1,a1,-1
   14a74:	8c09                	sub	s0,s0,a0
   14a76:	8df9                	and	a1,a1,a4
   14a78:	84d2                	mv	s1,s4
   14a7a:	b711                	j	1497e <__subtf3+0x362>
   14a7c:	8526                	mv	a0,s1
   14a7e:	0e5000ef          	jal	ra,15362 <__clzdi2>
   14a82:	ff45071b          	addiw	a4,a0,-12
   14a86:	03f00793          	li	a5,63
   14a8a:	0345051b          	addiw	a0,a0,52
   14a8e:	00e49733          	sll	a4,s1,a4
   14a92:	c8a7c1e3          	blt	a5,a0,14714 <__subtf3+0xf8>
   14a96:	b1ad                	j	14700 <__subtf3+0xe4>
   14a98:	f00919e3          	bnez	s2,149aa <__subtf3+0x38e>
   14a9c:	0058e893          	ori	a7,a7,5
   14aa0:	6721                	lui	a4,0x8
   14aa2:	fff70413          	addi	s0,a4,-1 # 7fff <exit-0x8121>
   14aa6:	00197693          	andi	a3,s2,1
   14aaa:	4701                	li	a4,0
   14aac:	4501                	li	a0,0
   14aae:	b32d                	j	147d8 <__subtf3+0x1bc>
   14ab0:	67a1                	lui	a5,0x8
   14ab2:	17fd                	addi	a5,a5,-1
   14ab4:	84aa                	mv	s1,a0
   14ab6:	ecf414e3          	bne	s0,a5,1497e <__subtf3+0x362>
   14aba:	00a5e7b3          	or	a5,a1,a0
   14abe:	e60799e3          	bnez	a5,14930 <__subtf3+0x314>
   14ac2:	4501                	li	a0,0
   14ac4:	4781                	li	a5,0
   14ac6:	4881                	li	a7,0
   14ac8:	bd41                	j	14958 <__subtf3+0x33c>
   14aca:	26830063          	beq	t1,s0,14d2a <__subtf3+0x70e>
   14ace:	26079663          	bnez	a5,14d3a <__subtf3+0x71e>
   14ad2:	0ffff5b7          	lui	a1,0xffff
   14ad6:	4501                	li	a0,0
   14ad8:	158e                	slli	a1,a1,0x23
   14ada:	48c1                	li	a7,16
   14adc:	0018a073          	csrs	fflags,a7
   14ae0:	70a2                	ld	ra,40(sp)
   14ae2:	7402                	ld	s0,32(sp)
   14ae4:	64e2                	ld	s1,24(sp)
   14ae6:	6942                	ld	s2,16(sp)
   14ae8:	69a2                	ld	s3,8(sp)
   14aea:	6a02                	ld	s4,0(sp)
   14aec:	6145                	addi	sp,sp,48
   14aee:	8082                	ret
   14af0:	67a1                	lui	a5,0x8
   14af2:	00140713          	addi	a4,s0,1 # 8001 <exit-0x811f>
   14af6:	ffe78693          	addi	a3,a5,-2 # 7ffe <exit-0x8122>
   14afa:	00d77833          	and	a6,a4,a3
   14afe:	1a081463          	bnez	a6,14ca6 <__subtf3+0x68a>
   14b02:	00a5e733          	or	a4,a1,a0
   14b06:	36041c63          	bnez	s0,14e7e <__subtf3+0x862>
   14b0a:	01cee7b3          	or	a5,t4,t3
   14b0e:	40070063          	beqz	a4,14f0e <__subtf3+0x8f2>
   14b12:	28078263          	beqz	a5,14d96 <__subtf3+0x77a>
   14b16:	01c504b3          	add	s1,a0,t3
   14b1a:	01d58733          	add	a4,a1,t4
   14b1e:	00a4b533          	sltu	a0,s1,a0
   14b22:	00a705b3          	add	a1,a4,a0
   14b26:	00c59793          	slli	a5,a1,0xc
   14b2a:	c007dde3          	bgez	a5,14744 <__subtf3+0x128>
   14b2e:	57fd                	li	a5,-1
   14b30:	17ce                	slli	a5,a5,0x33
   14b32:	17fd                	addi	a5,a5,-1
   14b34:	8dfd                	and	a1,a1,a5
   14b36:	4405                	li	s0,1
   14b38:	0074f793          	andi	a5,s1,7
   14b3c:	b5b9                	j	1498a <__subtf3+0x36e>
   14b3e:	67a1                	lui	a5,0x8
   14b40:	17fd                	addi	a5,a5,-1
   14b42:	36f30263          	beq	t1,a5,14ea6 <__subtf3+0x88a>
   14b46:	4785                	li	a5,1
   14b48:	17ce                	slli	a5,a5,0x33
   14b4a:	41e00f3b          	negw	t5,t5
   14b4e:	8ddd                	or	a1,a1,a5
   14b50:	07400793          	li	a5,116
   14b54:	0fe7cb63          	blt	a5,t5,14c4a <__subtf3+0x62e>
   14b58:	03f00793          	li	a5,63
   14b5c:	2fe7ce63          	blt	a5,t5,14e58 <__subtf3+0x83c>
   14b60:	04000793          	li	a5,64
   14b64:	41e787bb          	subw	a5,a5,t5
   14b68:	01e55733          	srl	a4,a0,t5
   14b6c:	00f594b3          	sll	s1,a1,a5
   14b70:	00f51533          	sll	a0,a0,a5
   14b74:	8cd9                	or	s1,s1,a4
   14b76:	00a03533          	snez	a0,a0
   14b7a:	01e5df33          	srl	t5,a1,t5
   14b7e:	8cc9                	or	s1,s1,a0
   14b80:	41ee8eb3          	sub	t4,t4,t5
   14b84:	a0f1                	j	14c50 <__subtf3+0x634>
   14b86:	41c504b3          	sub	s1,a0,t3
   14b8a:	41d58a33          	sub	s4,a1,t4
   14b8e:	00953733          	sltu	a4,a0,s1
   14b92:	40ea0a33          	sub	s4,s4,a4
   14b96:	00ca1793          	slli	a5,s4,0xc
   14b9a:	1407c963          	bltz	a5,14cec <__subtf3+0x6d0>
   14b9e:	0144e7b3          	or	a5,s1,s4
   14ba2:	b40799e3          	bnez	a5,146f4 <__subtf3+0xd8>
   14ba6:	ffe98693          	addi	a3,s3,-2
   14baa:	0016b913          	seqz	s2,a3
   14bae:	4501                	li	a0,0
   14bb0:	4401                	li	s0,0
   14bb2:	b3b9                	j	14900 <__subtf3+0x2e4>
   14bb4:	01cee4b3          	or	s1,t4,t3
   14bb8:	009034b3          	snez	s1,s1
   14bbc:	94aa                	add	s1,s1,a0
   14bbe:	00a4b533          	sltu	a0,s1,a0
   14bc2:	95aa                	add	a1,a1,a0
   14bc4:	00c59793          	slli	a5,a1,0xc
   14bc8:	da07dbe3          	bgez	a5,1497e <__subtf3+0x362>
   14bcc:	67a1                	lui	a5,0x8
   14bce:	0405                	addi	s0,s0,1
   14bd0:	fff78713          	addi	a4,a5,-1 # 7fff <exit-0x8121>
   14bd4:	1ee40e63          	beq	s0,a4,14dd0 <__subtf3+0x7b4>
   14bd8:	577d                	li	a4,-1
   14bda:	174e                	slli	a4,a4,0x33
   14bdc:	177d                	addi	a4,a4,-1
   14bde:	8f6d                	and	a4,a4,a1
   14be0:	0014d793          	srli	a5,s1,0x1
   14be4:	8885                	andi	s1,s1,1
   14be6:	8fc5                	or	a5,a5,s1
   14be8:	03f71493          	slli	s1,a4,0x3f
   14bec:	8cdd                	or	s1,s1,a5
   14bee:	00175593          	srli	a1,a4,0x1
   14bf2:	8b9d                	andi	a5,a5,7
   14bf4:	bb59                	j	1498a <__subtf3+0x36e>
   14bf6:	fc15051b          	addiw	a0,a0,-63
   14bfa:	04000793          	li	a5,64
   14bfe:	00a75533          	srl	a0,a4,a0
   14c02:	00f58a63          	beq	a1,a5,14c16 <__subtf3+0x5fa>
   14c06:	08000793          	li	a5,128
   14c0a:	40b786bb          	subw	a3,a5,a1
   14c0e:	00d716b3          	sll	a3,a4,a3
   14c12:	00da6a33          	or	s4,s4,a3
   14c16:	014037b3          	snez	a5,s4
   14c1a:	8fc9                	or	a5,a5,a0
   14c1c:	84be                	mv	s1,a5
   14c1e:	4581                	li	a1,0
   14c20:	b625                	j	14748 <__subtf3+0x12c>
   14c22:	fc08849b          	addiw	s1,a7,-64
   14c26:	04000793          	li	a5,64
   14c2a:	009ed733          	srl	a4,t4,s1
   14c2e:	00f88a63          	beq	a7,a5,14c42 <__subtf3+0x626>
   14c32:	08000793          	li	a5,128
   14c36:	411788bb          	subw	a7,a5,a7
   14c3a:	011e98b3          	sll	a7,t4,a7
   14c3e:	011e6e33          	or	t3,t3,a7
   14c42:	01c034b3          	snez	s1,t3
   14c46:	8cd9                	or	s1,s1,a4
   14c48:	bc49                	j	146da <__subtf3+0xbe>
   14c4a:	8dc9                	or	a1,a1,a0
   14c4c:	00b034b3          	snez	s1,a1
   14c50:	409e04b3          	sub	s1,t3,s1
   14c54:	009e3733          	sltu	a4,t3,s1
   14c58:	40ee85b3          	sub	a1,t4,a4
   14c5c:	841a                	mv	s0,t1
   14c5e:	8936                	mv	s2,a3
   14c60:	b451                	j	146e4 <__subtf3+0xc8>
   14c62:	00a5e7b3          	or	a5,a1,a0
   14c66:	28078663          	beqz	a5,14ef2 <__subtf3+0x8d6>
   14c6a:	577d                	li	a4,-1
   14c6c:	3ee88a63          	beq	a7,a4,15060 <__subtf3+0xa44>
   14c70:	67a1                	lui	a5,0x8
   14c72:	17fd                	addi	a5,a5,-1
   14c74:	38f30463          	beq	t1,a5,14ffc <__subtf3+0x9e0>
   14c78:	ffff4f13          	not	t5,t5
   14c7c:	2f01                	sext.w	t5,t5
   14c7e:	bb99                	j	149d4 <__subtf3+0x3b8>
   14c80:	04000793          	li	a5,64
   14c84:	411787bb          	subw	a5,a5,a7
   14c88:	011e5733          	srl	a4,t3,a7
   14c8c:	00fe94b3          	sll	s1,t4,a5
   14c90:	00fe1e33          	sll	t3,t3,a5
   14c94:	8cd9                	or	s1,s1,a4
   14c96:	01c03e33          	snez	t3,t3
   14c9a:	011ed8b3          	srl	a7,t4,a7
   14c9e:	01c4e4b3          	or	s1,s1,t3
   14ca2:	95c6                	add	a1,a1,a7
   14ca4:	bf21                	j	14bbc <__subtf3+0x5a0>
   14ca6:	17fd                	addi	a5,a5,-1
   14ca8:	2af70563          	beq	a4,a5,14f52 <__subtf3+0x936>
   14cac:	01c507b3          	add	a5,a0,t3
   14cb0:	00a7b533          	sltu	a0,a5,a0
   14cb4:	95f6                	add	a1,a1,t4
   14cb6:	95aa                	add	a1,a1,a0
   14cb8:	8385                	srli	a5,a5,0x1
   14cba:	03f59493          	slli	s1,a1,0x3f
   14cbe:	8cdd                	or	s1,s1,a5
   14cc0:	8185                	srli	a1,a1,0x1
   14cc2:	8b9d                	andi	a5,a5,7
   14cc4:	843a                	mv	s0,a4
   14cc6:	b1d1                	j	1498a <__subtf3+0x36e>
   14cc8:	177d                	addi	a4,a4,-1
   14cca:	04e40563          	beq	s0,a4,14d14 <__subtf3+0x6f8>
   14cce:	04e30e63          	beq	t1,a4,14d2a <__subtf3+0x70e>
   14cd2:	de080ee3          	beqz	a6,14ace <__subtf3+0x4b2>
   14cd6:	c80794e3          	bnez	a5,1495e <__subtf3+0x342>
   14cda:	003f9513          	slli	a0,t6,0x3
   14cde:	03d59793          	slli	a5,a1,0x3d
   14ce2:	810d                	srli	a0,a0,0x3
   14ce4:	8d5d                	or	a0,a0,a5
   14ce6:	0035d793          	srli	a5,a1,0x3
   14cea:	b1bd                	j	14958 <__subtf3+0x33c>
   14cec:	40ae04b3          	sub	s1,t3,a0
   14cf0:	40be85b3          	sub	a1,t4,a1
   14cf4:	009e3e33          	sltu	t3,t3,s1
   14cf8:	41c58a33          	sub	s4,a1,t3
   14cfc:	8936                	mv	s2,a3
   14cfe:	badd                	j	146f4 <__subtf3+0xd8>
   14d00:	01c504b3          	add	s1,a0,t3
   14d04:	95f6                	add	a1,a1,t4
   14d06:	00a4b733          	sltu	a4,s1,a0
   14d0a:	95ba                	add	a1,a1,a4
   14d0c:	bd65                	j	14bc4 <__subtf3+0x5a8>
   14d0e:	c8090ee3          	beqz	s2,149aa <__subtf3+0x38e>
   14d12:	b369                	j	14a9c <__subtf3+0x480>
   14d14:	da080be3          	beqz	a6,14aca <__subtf3+0x4ae>
   14d18:	00d59713          	slli	a4,a1,0xd
   14d1c:	00074363          	bltz	a4,14d22 <__subtf3+0x706>
   14d20:	48c1                	li	a7,16
   14d22:	6721                	lui	a4,0x8
   14d24:	177d                	addi	a4,a4,-1
   14d26:	fae318e3          	bne	t1,a4,14cd6 <__subtf3+0x6ba>
   14d2a:	d7c5                	beqz	a5,14cd2 <__subtf3+0x6b6>
   14d2c:	00de9793          	slli	a5,t4,0xd
   14d30:	0007c363          	bltz	a5,14d36 <__subtf3+0x71a>
   14d34:	48c1                	li	a7,16
   14d36:	c20814e3          	bnez	a6,1495e <__subtf3+0x342>
   14d3a:	00361513          	slli	a0,a2,0x3
   14d3e:	03de9793          	slli	a5,t4,0x3d
   14d42:	810d                	srli	a0,a0,0x3
   14d44:	8d5d                	or	a0,a0,a5
   14d46:	8936                	mv	s2,a3
   14d48:	003ed793          	srli	a5,t4,0x3
   14d4c:	b131                	j	14958 <__subtf3+0x33c>
   14d4e:	8936                	mv	s2,a3
   14d50:	4881                	li	a7,0
   14d52:	b3b9                	j	14aa0 <__subtf3+0x484>
   14d54:	41c504b3          	sub	s1,a0,t3
   14d58:	41d58733          	sub	a4,a1,t4
   14d5c:	00953533          	sltu	a0,a0,s1
   14d60:	40a705b3          	sub	a1,a4,a0
   14d64:	b241                	j	146e4 <__subtf3+0xc8>
   14d66:	8d4d                	or	a0,a0,a1
   14d68:	d565                	beqz	a0,14d50 <__subtf3+0x734>
   14d6a:	0325d893          	srli	a7,a1,0x32
   14d6e:	0f8e                	slli	t6,t6,0x3
   14d70:	0018f893          	andi	a7,a7,1
   14d74:	03d59513          	slli	a0,a1,0x3d
   14d78:	003fd493          	srli	s1,t6,0x3
   14d7c:	0018b893          	seqz	a7,a7
   14d80:	8d45                	or	a0,a0,s1
   14d82:	0035d793          	srli	a5,a1,0x3
   14d86:	0892                	slli	a7,a7,0x4
   14d88:	bec1                	j	14958 <__subtf3+0x33c>
   14d8a:	16078c63          	beqz	a5,14f02 <__subtf3+0x8e6>
   14d8e:	4781                	li	a5,0
   14d90:	85f6                	mv	a1,t4
   14d92:	8572                	mv	a0,t3
   14d94:	8936                	mv	s2,a3
   14d96:	84aa                	mv	s1,a0
   14d98:	ba65                	j	14750 <__subtf3+0x134>
   14d9a:	00f67713          	andi	a4,a2,15
   14d9e:	4511                	li	a0,4
   14da0:	9ca70ce3          	beq	a4,a0,14778 <__subtf3+0x15c>
   14da4:	ffc63713          	sltiu	a4,a2,-4
   14da8:	00174713          	xori	a4,a4,1
   14dac:	1702                	slli	a4,a4,0x20
   14dae:	9301                	srli	a4,a4,0x20
   14db0:	96ba                	add	a3,a3,a4
   14db2:	b2d9                	j	14778 <__subtf3+0x15c>
   14db4:	9c0912e3          	bnez	s2,14778 <__subtf3+0x15c>
   14db8:	ff863713          	sltiu	a4,a2,-8
   14dbc:	00174713          	xori	a4,a4,1
   14dc0:	1702                	slli	a4,a4,0x20
   14dc2:	9301                	srli	a4,a4,0x20
   14dc4:	96ba                	add	a3,a3,a4
   14dc6:	ba4d                	j	14778 <__subtf3+0x15c>
   14dc8:	4885                	li	a7,1
   14dca:	9a0907e3          	beqz	s2,14778 <__subtf3+0x15c>
   14dce:	b7ed                	j	14db8 <__subtf3+0x79c>
   14dd0:	04098c63          	beqz	s3,14e28 <__subtf3+0x80c>
   14dd4:	470d                	li	a4,3
   14dd6:	26e98c63          	beq	s3,a4,1504e <__subtf3+0xa32>
   14dda:	4709                	li	a4,2
   14ddc:	4895                	li	a7,5
   14dde:	bce990e3          	bne	s3,a4,1499e <__subtf3+0x382>
   14de2:	ca091fe3          	bnez	s2,14aa0 <__subtf3+0x484>
   14de6:	ffe78413          	addi	s0,a5,-2 # 7ffe <exit-0x8122>
   14dea:	55fd                	li	a1,-1
   14dec:	54fd                	li	s1,-1
   14dee:	ba7d                	j	147ac <__subtf3+0x190>
   14df0:	8d4d                	or	a0,a0,a1
   14df2:	dd39                	beqz	a0,14d50 <__subtf3+0x734>
   14df4:	0325d893          	srli	a7,a1,0x32
   14df8:	003f9793          	slli	a5,t6,0x3
   14dfc:	0018f893          	andi	a7,a7,1
   14e00:	838d                	srli	a5,a5,0x3
   14e02:	03d59513          	slli	a0,a1,0x3d
   14e06:	0018b893          	seqz	a7,a7
   14e0a:	8d5d                	or	a0,a0,a5
   14e0c:	0892                	slli	a7,a7,0x4
   14e0e:	0035d793          	srli	a5,a1,0x3
   14e12:	b699                	j	14958 <__subtf3+0x33c>
   14e14:	174e                	slli	a4,a4,0x33
   14e16:	8f6d                	and	a4,a4,a1
   14e18:	980698e3          	bnez	a3,147a8 <__subtf3+0x18c>
   14e1c:	843e                	mv	s0,a5
   14e1e:	980885e3          	beqz	a7,147a8 <__subtf3+0x18c>
   14e22:	0028e893          	ori	a7,a7,2
   14e26:	b249                	j	147a8 <__subtf3+0x18c>
   14e28:	4895                	li	a7,5
   14e2a:	b99d                	j	14aa0 <__subtf3+0x484>
   14e2c:	8d4d                	or	a0,a0,a1
   14e2e:	d10d                	beqz	a0,14d50 <__subtf3+0x734>
   14e30:	0325d893          	srli	a7,a1,0x32
   14e34:	003f9513          	slli	a0,t6,0x3
   14e38:	0018f893          	andi	a7,a7,1
   14e3c:	03d59793          	slli	a5,a1,0x3d
   14e40:	810d                	srli	a0,a0,0x3
   14e42:	0018b893          	seqz	a7,a7
   14e46:	8d5d                	or	a0,a0,a5
   14e48:	0892                	slli	a7,a7,0x4
   14e4a:	0035d793          	srli	a5,a1,0x3
   14e4e:	b629                	j	14958 <__subtf3+0x33c>
   14e50:	8dc9                	or	a1,a1,a0
   14e52:	00b034b3          	snez	s1,a1
   14e56:	be45                	j	14a06 <__subtf3+0x3ea>
   14e58:	fc0f049b          	addiw	s1,t5,-64
   14e5c:	04000713          	li	a4,64
   14e60:	0095d7b3          	srl	a5,a1,s1
   14e64:	00ef0963          	beq	t5,a4,14e76 <__subtf3+0x85a>
   14e68:	08000713          	li	a4,128
   14e6c:	41e70f3b          	subw	t5,a4,t5
   14e70:	01e59733          	sll	a4,a1,t5
   14e74:	8d59                	or	a0,a0,a4
   14e76:	00a034b3          	snez	s1,a0
   14e7a:	8cdd                	or	s1,s1,a5
   14e7c:	bbd1                	j	14c50 <__subtf3+0x634>
   14e7e:	17fd                	addi	a5,a5,-1
   14e80:	1af40263          	beq	s0,a5,15024 <__subtf3+0xa08>
   14e84:	1ef30a63          	beq	t1,a5,15078 <__subtf3+0xa5c>
   14e88:	12070363          	beqz	a4,14fae <__subtf3+0x992>
   14e8c:	01cee7b3          	or	a5,t4,t3
   14e90:	ac0797e3          	bnez	a5,1495e <__subtf3+0x342>
   14e94:	003f9793          	slli	a5,t6,0x3
   14e98:	838d                	srli	a5,a5,0x3
   14e9a:	03d59513          	slli	a0,a1,0x3d
   14e9e:	8d5d                	or	a0,a0,a5
   14ea0:	0035d793          	srli	a5,a1,0x3
   14ea4:	bc55                	j	14958 <__subtf3+0x33c>
   14ea6:	01ceee33          	or	t3,t4,t3
   14eaa:	ea0e02e3          	beqz	t3,14d4e <__subtf3+0x732>
   14eae:	00361513          	slli	a0,a2,0x3
   14eb2:	03de9793          	slli	a5,t4,0x3d
   14eb6:	810d                	srli	a0,a0,0x3
   14eb8:	00de9713          	slli	a4,t4,0xd
   14ebc:	8d5d                	or	a0,a0,a5
   14ebe:	8936                	mv	s2,a3
   14ec0:	003ed793          	srli	a5,t4,0x3
   14ec4:	c00741e3          	bltz	a4,14ac6 <__subtf3+0x4aa>
   14ec8:	48c1                	li	a7,16
   14eca:	b479                	j	14958 <__subtf3+0x33c>
   14ecc:	01ceee33          	or	t3,t4,t3
   14ed0:	e60e0fe3          	beqz	t3,14d4e <__subtf3+0x732>
   14ed4:	00375513          	srli	a0,a4,0x3
   14ed8:	8d71                	and	a0,a0,a2
   14eda:	03de9713          	slli	a4,t4,0x3d
   14ede:	8d59                	or	a0,a0,a4
   14ee0:	00de9713          	slli	a4,t4,0xd
   14ee4:	003ed793          	srli	a5,t4,0x3
   14ee8:	8936                	mv	s2,a3
   14eea:	bc074ee3          	bltz	a4,14ac6 <__subtf3+0x4aa>
   14eee:	48c1                	li	a7,16
   14ef0:	b4a5                	j	14958 <__subtf3+0x33c>
   14ef2:	67a1                	lui	a5,0x8
   14ef4:	17fd                	addi	a5,a5,-1
   14ef6:	08f30a63          	beq	t1,a5,14f8a <__subtf3+0x96e>
   14efa:	85f6                	mv	a1,t4
   14efc:	84f2                	mv	s1,t3
   14efe:	841a                	mv	s0,t1
   14f00:	bcbd                	j	1497e <__subtf3+0x362>
   14f02:	ffe98693          	addi	a3,s3,-2
   14f06:	0016b913          	seqz	s2,a3
   14f0a:	4501                	li	a0,0
   14f0c:	bad5                	j	14900 <__subtf3+0x2e4>
   14f0e:	85f6                	mv	a1,t4
   14f10:	84f2                	mv	s1,t3
   14f12:	b81d                	j	14748 <__subtf3+0x12c>
   14f14:	fc0f079b          	addiw	a5,t5,-64
   14f18:	04000713          	li	a4,64
   14f1c:	00f5d7b3          	srl	a5,a1,a5
   14f20:	00ef0963          	beq	t5,a4,14f32 <__subtf3+0x916>
   14f24:	08000713          	li	a4,128
   14f28:	41e70f3b          	subw	t5,a4,t5
   14f2c:	01e59733          	sll	a4,a1,t5
   14f30:	8d59                	or	a0,a0,a4
   14f32:	00a034b3          	snez	s1,a0
   14f36:	8cdd                	or	s1,s1,a5
   14f38:	b4f9                	j	14a06 <__subtf3+0x3ea>
   14f3a:	40ae04b3          	sub	s1,t3,a0
   14f3e:	40be8733          	sub	a4,t4,a1
   14f42:	009e3e33          	sltu	t3,t3,s1
   14f46:	41c705b3          	sub	a1,a4,t3
   14f4a:	841a                	mv	s0,t1
   14f4c:	8936                	mv	s2,a3
   14f4e:	f96ff06f          	j	146e4 <__subtf3+0xc8>
   14f52:	ec098be3          	beqz	s3,14e28 <__subtf3+0x80c>
   14f56:	478d                	li	a5,3
   14f58:	12f98763          	beq	s3,a5,15086 <__subtf3+0xa6a>
   14f5c:	4789                	li	a5,2
   14f5e:	4895                	li	a7,5
   14f60:	a4f995e3          	bne	s3,a5,149aa <__subtf3+0x38e>
   14f64:	b2091ee3          	bnez	s2,14aa0 <__subtf3+0x484>
   14f68:	8436                	mv	s0,a3
   14f6a:	55fd                	li	a1,-1
   14f6c:	54fd                	li	s1,-1
   14f6e:	b83d                	j	147ac <__subtf3+0x190>
   14f70:	01cee7b3          	or	a5,t4,t3
   14f74:	10078063          	beqz	a5,15074 <__subtf3+0xa58>
   14f78:	00de9793          	slli	a5,t4,0xd
   14f7c:	85f6                	mv	a1,t4
   14f7e:	8572                	mv	a0,t3
   14f80:	8936                	mv	s2,a3
   14f82:	0007ce63          	bltz	a5,14f9e <__subtf3+0x982>
   14f86:	48c1                	li	a7,16
   14f88:	ba5d                	j	1493e <__subtf3+0x322>
   14f8a:	01cee7b3          	or	a5,t4,t3
   14f8e:	b2078ae3          	beqz	a5,14ac2 <__subtf3+0x4a6>
   14f92:	00de9793          	slli	a5,t4,0xd
   14f96:	85f6                	mv	a1,t4
   14f98:	8572                	mv	a0,t3
   14f9a:	fe07d6e3          	bgez	a5,14f86 <__subtf3+0x96a>
   14f9e:	4881                	li	a7,0
   14fa0:	ba79                	j	1493e <__subtf3+0x322>
   14fa2:	00831663          	bne	t1,s0,14fae <__subtf3+0x992>
   14fa6:	01ceee33          	or	t3,t4,t3
   14faa:	080e1b63          	bnez	t3,15040 <__subtf3+0xa24>
   14fae:	00361793          	slli	a5,a2,0x3
   14fb2:	838d                	srli	a5,a5,0x3
   14fb4:	03de9513          	slli	a0,t4,0x3d
   14fb8:	8d5d                	or	a0,a0,a5
   14fba:	003ed793          	srli	a5,t4,0x3
   14fbe:	ba69                	j	14958 <__subtf3+0x33c>
   14fc0:	00e667b3          	or	a5,a2,a4
   14fc4:	df9d                	beqz	a5,14f02 <__subtf3+0x8e6>
   14fc6:	8532                	mv	a0,a2
   14fc8:	00767793          	andi	a5,a2,7
   14fcc:	85ba                	mv	a1,a4
   14fce:	84aa                	mv	s1,a0
   14fd0:	f80ff06f          	j	14750 <__subtf3+0x134>
   14fd4:	01ceee33          	or	t3,t4,t3
   14fd8:	d60e0ce3          	beqz	t3,14d50 <__subtf3+0x734>
   14fdc:	032ed893          	srli	a7,t4,0x32
   14fe0:	00361513          	slli	a0,a2,0x3
   14fe4:	0018f893          	andi	a7,a7,1
   14fe8:	03de9793          	slli	a5,t4,0x3d
   14fec:	810d                	srli	a0,a0,0x3
   14fee:	0018b893          	seqz	a7,a7
   14ff2:	8d5d                	or	a0,a0,a5
   14ff4:	0892                	slli	a7,a7,0x4
   14ff6:	003ed793          	srli	a5,t4,0x3
   14ffa:	bab9                	j	14958 <__subtf3+0x33c>
   14ffc:	01ceee33          	or	t3,t4,t3
   15000:	d40e08e3          	beqz	t3,14d50 <__subtf3+0x734>
   15004:	032ed893          	srli	a7,t4,0x32
   15008:	00375513          	srli	a0,a4,0x3
   1500c:	0018f893          	andi	a7,a7,1
   15010:	03de9713          	slli	a4,t4,0x3d
   15014:	8d71                	and	a0,a0,a2
   15016:	0018b893          	seqz	a7,a7
   1501a:	8d59                	or	a0,a0,a4
   1501c:	003ed793          	srli	a5,t4,0x3
   15020:	0892                	slli	a7,a7,0x4
   15022:	ba1d                	j	14958 <__subtf3+0x33c>
   15024:	df3d                	beqz	a4,14fa2 <__subtf3+0x986>
   15026:	00d59793          	slli	a5,a1,0xd
   1502a:	0007c363          	bltz	a5,15030 <__subtf3+0xa14>
   1502e:	48c1                	li	a7,16
   15030:	67a1                	lui	a5,0x8
   15032:	17fd                	addi	a5,a5,-1
   15034:	e4f31ce3          	bne	t1,a5,14e8c <__subtf3+0x870>
   15038:	01ceee33          	or	t3,t4,t3
   1503c:	e40e0ce3          	beqz	t3,14e94 <__subtf3+0x878>
   15040:	00de9793          	slli	a5,t4,0xd
   15044:	0007c363          	bltz	a5,1504a <__subtf3+0xa2e>
   15048:	48c1                	li	a7,16
   1504a:	d335                	beqz	a4,14fae <__subtf3+0x992>
   1504c:	ba09                	j	1495e <__subtf3+0x342>
   1504e:	dc090de3          	beqz	s2,14e28 <__subtf3+0x80c>
   15052:	54fd                	li	s1,-1
   15054:	ffe78413          	addi	s0,a5,-2 # 7ffe <exit-0x8122>
   15058:	55fd                	li	a1,-1
   1505a:	4895                	li	a7,5
   1505c:	4601                	li	a2,0
   1505e:	b409                	j	14a60 <__subtf3+0x444>
   15060:	01c504b3          	add	s1,a0,t3
   15064:	01d58733          	add	a4,a1,t4
   15068:	01c4be33          	sltu	t3,s1,t3
   1506c:	01c705b3          	add	a1,a4,t3
   15070:	841a                	mv	s0,t1
   15072:	be89                	j	14bc4 <__subtf3+0x5a8>
   15074:	8936                	mv	s2,a3
   15076:	b4b1                	j	14ac2 <__subtf3+0x4a6>
   15078:	01ceee33          	or	t3,t4,t3
   1507c:	fc0e12e3          	bnez	t3,15040 <__subtf3+0xa24>
   15080:	e0071ae3          	bnez	a4,14e94 <__subtf3+0x878>
   15084:	b72d                	j	14fae <__subtf3+0x992>
   15086:	da0901e3          	beqz	s2,14e28 <__subtf3+0x80c>
   1508a:	4601                	li	a2,0
   1508c:	54fd                	li	s1,-1
   1508e:	8436                	mv	s0,a3
   15090:	55fd                	li	a1,-1
   15092:	4895                	li	a7,5
   15094:	b2f1                	j	14a60 <__subtf3+0x444>

0000000000015096 <__fixtfsi>:
   15096:	882a                	mv	a6,a0
   15098:	002027f3          	frrm	a5
   1509c:	6611                	lui	a2,0x4
   1509e:	00159793          	slli	a5,a1,0x1
   150a2:	01059713          	slli	a4,a1,0x10
   150a6:	93c5                	srli	a5,a5,0x31
   150a8:	ffe60693          	addi	a3,a2,-2 # 3ffe <exit-0xc122>
   150ac:	8341                	srli	a4,a4,0x10
   150ae:	91fd                	srli	a1,a1,0x3f
   150b0:	00f6cf63          	blt	a3,a5,150ce <__fixtfsi+0x38>
   150b4:	eb81                	bnez	a5,150c4 <__fixtfsi+0x2e>
   150b6:	8f49                	or	a4,a4,a0
   150b8:	4501                	li	a0,0
   150ba:	cb3d                	beqz	a4,15130 <__fixtfsi+0x9a>
   150bc:	4685                	li	a3,1
   150be:	0016a073          	csrs	fflags,a3
   150c2:	8082                	ret
   150c4:	4501                	li	a0,0
   150c6:	4685                	li	a3,1
   150c8:	0016a073          	csrs	fflags,a3
   150cc:	bfdd                	j	150c2 <__fixtfsi+0x2c>
   150ce:	01d60693          	addi	a3,a2,29
   150d2:	02f6d663          	bge	a3,a5,150fe <__fixtfsi+0x68>
   150d6:	800006b7          	lui	a3,0x80000
   150da:	fff6c693          	not	a3,a3
   150de:	00b6853b          	addw	a0,a3,a1
   150e2:	c9a1                	beqz	a1,15132 <__fixtfsi+0x9c>
   150e4:	0679                	addi	a2,a2,30
   150e6:	46c1                	li	a3,16
   150e8:	fcc79be3          	bne	a5,a2,150be <__fixtfsi+0x28>
   150ec:	01175793          	srli	a5,a4,0x11
   150f0:	f7f9                	bnez	a5,150be <__fixtfsi+0x28>
   150f2:	02f71693          	slli	a3,a4,0x2f
   150f6:	0106e7b3          	or	a5,a3,a6
   150fa:	f3e9                	bnez	a5,150bc <__fixtfsi+0x26>
   150fc:	8082                	ret
   150fe:	4685                	li	a3,1
   15100:	16c2                	slli	a3,a3,0x30
   15102:	8f55                	or	a4,a4,a3
   15104:	6611                	lui	a2,0x4
   15106:	76f1                	lui	a3,0xffffc
   15108:	26c5                	addiw	a3,a3,17
   1510a:	02f6061b          	addiw	a2,a2,47
   1510e:	9ebd                	addw	a3,a3,a5
   15110:	40f607bb          	subw	a5,a2,a5
   15114:	00d716b3          	sll	a3,a4,a3
   15118:	00f75733          	srl	a4,a4,a5
   1511c:	2701                	sext.w	a4,a4
   1511e:	00a6e7b3          	or	a5,a3,a0
   15122:	c199                	beqz	a1,15128 <__fixtfsi+0x92>
   15124:	40e0073b          	negw	a4,a4
   15128:	0007051b          	sext.w	a0,a4
   1512c:	fbc1                	bnez	a5,150bc <__fixtfsi+0x26>
   1512e:	8082                	ret
   15130:	8082                	ret
   15132:	46c1                	li	a3,16
   15134:	0016a073          	csrs	fflags,a3
   15138:	b769                	j	150c2 <__fixtfsi+0x2c>

000000000001513a <__fixunstfsi>:
   1513a:	002027f3          	frrm	a5
   1513e:	6611                	lui	a2,0x4
   15140:	00159793          	slli	a5,a1,0x1
   15144:	01059713          	slli	a4,a1,0x10
   15148:	93c5                	srli	a5,a5,0x31
   1514a:	ffe60693          	addi	a3,a2,-2 # 3ffe <exit-0xc122>
   1514e:	8341                	srli	a4,a4,0x10
   15150:	91fd                	srli	a1,a1,0x3f
   15152:	00f6ca63          	blt	a3,a5,15166 <__fixunstfsi+0x2c>
   15156:	e399                	bnez	a5,1515c <__fixunstfsi+0x22>
   15158:	8f49                	or	a4,a4,a0
   1515a:	cf29                	beqz	a4,151b4 <__fixunstfsi+0x7a>
   1515c:	4785                	li	a5,1
   1515e:	4501                	li	a0,0
   15160:	0017a073          	csrs	fflags,a5
   15164:	8082                	ret
   15166:	0015b693          	seqz	a3,a1
   1516a:	96b2                	add	a3,a3,a2
   1516c:	06f9                	addi	a3,a3,30
   1516e:	00d7c863          	blt	a5,a3,1517e <__fixunstfsi+0x44>
   15172:	fff58513          	addi	a0,a1,-1 # fffefff <__global_pointer$+0xffe67ff>
   15176:	47c1                	li	a5,16
   15178:	0017a073          	csrs	fflags,a5
   1517c:	b7e5                	j	15164 <__fixunstfsi+0x2a>
   1517e:	c591                	beqz	a1,1518a <__fixunstfsi+0x50>
   15180:	47c1                	li	a5,16
   15182:	4501                	li	a0,0
   15184:	0017a073          	csrs	fflags,a5
   15188:	bff1                	j	15164 <__fixunstfsi+0x2a>
   1518a:	4585                	li	a1,1
   1518c:	15c2                	slli	a1,a1,0x30
   1518e:	8dd9                	or	a1,a1,a4
   15190:	7771                	lui	a4,0xffffc
   15192:	2745                	addiw	a4,a4,17
   15194:	6691                	lui	a3,0x4
   15196:	9f3d                	addw	a4,a4,a5
   15198:	02f6869b          	addiw	a3,a3,47
   1519c:	40f687bb          	subw	a5,a3,a5
   151a0:	00e59733          	sll	a4,a1,a4
   151a4:	8f49                	or	a4,a4,a0
   151a6:	00f5d5b3          	srl	a1,a1,a5
   151aa:	0005851b          	sext.w	a0,a1
   151ae:	4785                	li	a5,1
   151b0:	fb45                	bnez	a4,15160 <__fixunstfsi+0x26>
   151b2:	8082                	ret
   151b4:	4501                	li	a0,0
   151b6:	8082                	ret

00000000000151b8 <__floatsitf>:
   151b8:	1101                	addi	sp,sp,-32
   151ba:	ec06                	sd	ra,24(sp)
   151bc:	e822                	sd	s0,16(sp)
   151be:	e426                	sd	s1,8(sp)
   151c0:	c539                	beqz	a0,1520e <__floatsitf+0x56>
   151c2:	0005059b          	sext.w	a1,a0
   151c6:	01f5549b          	srliw	s1,a0,0x1f
   151ca:	04054663          	bltz	a0,15216 <__floatsitf+0x5e>
   151ce:	02059413          	slli	s0,a1,0x20
   151d2:	9001                	srli	s0,s0,0x20
   151d4:	8522                	mv	a0,s0
   151d6:	18c000ef          	jal	ra,15362 <__clzdi2>
   151da:	6591                	lui	a1,0x4
   151dc:	03e5879b          	addiw	a5,a1,62
   151e0:	40a7853b          	subw	a0,a5,a0
   151e4:	02f5859b          	addiw	a1,a1,47
   151e8:	9d89                	subw	a1,a1,a0
   151ea:	00b415b3          	sll	a1,s0,a1
   151ee:	05c2                	slli	a1,a1,0x10
   151f0:	1546                	slli	a0,a0,0x31
   151f2:	81c1                	srli	a1,a1,0x10
   151f4:	9145                	srli	a0,a0,0x31
   151f6:	60e2                	ld	ra,24(sp)
   151f8:	6442                	ld	s0,16(sp)
   151fa:	04be                	slli	s1,s1,0xf
   151fc:	8cc9                	or	s1,s1,a0
   151fe:	05c2                	slli	a1,a1,0x10
   15200:	14c2                	slli	s1,s1,0x30
   15202:	81c1                	srli	a1,a1,0x10
   15204:	8dc5                	or	a1,a1,s1
   15206:	4501                	li	a0,0
   15208:	64a2                	ld	s1,8(sp)
   1520a:	6105                	addi	sp,sp,32
   1520c:	8082                	ret
   1520e:	4481                	li	s1,0
   15210:	4501                	li	a0,0
   15212:	4581                	li	a1,0
   15214:	b7cd                	j	151f6 <__floatsitf+0x3e>
   15216:	40b005bb          	negw	a1,a1
   1521a:	bf55                	j	151ce <__floatsitf+0x16>

000000000001521c <__floatunsitf>:
   1521c:	c121                	beqz	a0,1525c <__floatunsitf+0x40>
   1521e:	1141                	addi	sp,sp,-16
   15220:	e022                	sd	s0,0(sp)
   15222:	02051413          	slli	s0,a0,0x20
   15226:	9001                	srli	s0,s0,0x20
   15228:	8522                	mv	a0,s0
   1522a:	e406                	sd	ra,8(sp)
   1522c:	136000ef          	jal	ra,15362 <__clzdi2>
   15230:	6791                	lui	a5,0x4
   15232:	03e7859b          	addiw	a1,a5,62
   15236:	9d89                	subw	a1,a1,a0
   15238:	02f7879b          	addiw	a5,a5,47
   1523c:	9f8d                	subw	a5,a5,a1
   1523e:	00f417b3          	sll	a5,s0,a5
   15242:	07c2                	slli	a5,a5,0x10
   15244:	60a2                	ld	ra,8(sp)
   15246:	6402                	ld	s0,0(sp)
   15248:	15c6                	slli	a1,a1,0x31
   1524a:	83c1                	srli	a5,a5,0x10
   1524c:	91c5                	srli	a1,a1,0x31
   1524e:	07c2                	slli	a5,a5,0x10
   15250:	15c2                	slli	a1,a1,0x30
   15252:	83c1                	srli	a5,a5,0x10
   15254:	4501                	li	a0,0
   15256:	8ddd                	or	a1,a1,a5
   15258:	0141                	addi	sp,sp,16
   1525a:	8082                	ret
   1525c:	4781                	li	a5,0
   1525e:	4581                	li	a1,0
   15260:	07c2                	slli	a5,a5,0x10
   15262:	83c1                	srli	a5,a5,0x10
   15264:	15c2                	slli	a1,a1,0x30
   15266:	4501                	li	a0,0
   15268:	8ddd                	or	a1,a1,a5
   1526a:	8082                	ret

000000000001526c <__extenddftf2>:
   1526c:	1101                	addi	sp,sp,-32
   1526e:	e20505d3          	fmv.x.d	a1,fa0
   15272:	ec06                	sd	ra,24(sp)
   15274:	e822                	sd	s0,16(sp)
   15276:	e426                	sd	s1,8(sp)
   15278:	002027f3          	frrm	a5
   1527c:	0345d793          	srli	a5,a1,0x34
   15280:	7ff7f793          	andi	a5,a5,2047
   15284:	567d                	li	a2,-1
   15286:	00178693          	addi	a3,a5,1 # 4001 <exit-0xc11f>
   1528a:	00c65413          	srli	s0,a2,0xc
   1528e:	7fe6f693          	andi	a3,a3,2046
   15292:	8c6d                	and	s0,s0,a1
   15294:	03f5d493          	srli	s1,a1,0x3f
   15298:	c695                	beqz	a3,152c4 <__extenddftf2+0x58>
   1529a:	6711                	lui	a4,0x4
   1529c:	c0070713          	addi	a4,a4,-1024 # 3c00 <exit-0xc520>
   152a0:	97ba                	add	a5,a5,a4
   152a2:	17c6                	slli	a5,a5,0x31
   152a4:	00445593          	srli	a1,s0,0x4
   152a8:	93c5                	srli	a5,a5,0x31
   152aa:	1472                	slli	s0,s0,0x3c
   152ac:	17c6                	slli	a5,a5,0x31
   152ae:	8385                	srli	a5,a5,0x1
   152b0:	14fe                	slli	s1,s1,0x3f
   152b2:	8fcd                	or	a5,a5,a1
   152b4:	8522                	mv	a0,s0
   152b6:	0097e5b3          	or	a1,a5,s1
   152ba:	60e2                	ld	ra,24(sp)
   152bc:	6442                	ld	s0,16(sp)
   152be:	64a2                	ld	s1,8(sp)
   152c0:	6105                	addi	sp,sp,32
   152c2:	8082                	ret
   152c4:	ef85                	bnez	a5,152fc <__extenddftf2+0x90>
   152c6:	c035                	beqz	s0,1532a <__extenddftf2+0xbe>
   152c8:	8522                	mv	a0,s0
   152ca:	098000ef          	jal	ra,15362 <__clzdi2>
   152ce:	0005071b          	sext.w	a4,a0
   152d2:	47b9                	li	a5,14
   152d4:	08e7c163          	blt	a5,a4,15356 <__extenddftf2+0xea>
   152d8:	473d                	li	a4,15
   152da:	9f09                	subw	a4,a4,a0
   152dc:	0315079b          	addiw	a5,a0,49
   152e0:	00e45733          	srl	a4,s0,a4
   152e4:	00f41433          	sll	s0,s0,a5
   152e8:	6791                	lui	a5,0x4
   152ea:	c0c7879b          	addiw	a5,a5,-1012
   152ee:	9f89                	subw	a5,a5,a0
   152f0:	0742                	slli	a4,a4,0x10
   152f2:	17c6                	slli	a5,a5,0x31
   152f4:	01075593          	srli	a1,a4,0x10
   152f8:	93c5                	srli	a5,a5,0x31
   152fa:	bf4d                	j	152ac <__extenddftf2+0x40>
   152fc:	c815                	beqz	s0,15330 <__extenddftf2+0xc4>
   152fe:	4785                	li	a5,1
   15300:	02f79593          	slli	a1,a5,0x2f
   15304:	00445713          	srli	a4,s0,0x4
   15308:	17ce                	slli	a5,a5,0x33
   1530a:	8f4d                	or	a4,a4,a1
   1530c:	8fe1                	and	a5,a5,s0
   1530e:	01065593          	srli	a1,a2,0x10
   15312:	1472                	slli	s0,s0,0x3c
   15314:	8f6d                	and	a4,a4,a1
   15316:	c38d                	beqz	a5,15338 <__extenddftf2+0xcc>
   15318:	8f6d                	and	a4,a4,a1
   1531a:	07fff5b7          	lui	a1,0x7fff
   1531e:	1592                	slli	a1,a1,0x24
   15320:	14fe                	slli	s1,s1,0x3f
   15322:	8dd9                	or	a1,a1,a4
   15324:	8522                	mv	a0,s0
   15326:	8dc5                	or	a1,a1,s1
   15328:	bf49                	j	152ba <__extenddftf2+0x4e>
   1532a:	4781                	li	a5,0
   1532c:	4581                	li	a1,0
   1532e:	bfbd                	j	152ac <__extenddftf2+0x40>
   15330:	67a1                	lui	a5,0x8
   15332:	17fd                	addi	a5,a5,-1
   15334:	4581                	li	a1,0
   15336:	bf9d                	j	152ac <__extenddftf2+0x40>
   15338:	8f6d                	and	a4,a4,a1
   1533a:	07fff5b7          	lui	a1,0x7fff
   1533e:	1592                	slli	a1,a1,0x24
   15340:	14fe                	slli	s1,s1,0x3f
   15342:	8dd9                	or	a1,a1,a4
   15344:	8522                	mv	a0,s0
   15346:	8dc5                	or	a1,a1,s1
   15348:	00186073          	csrsi	fflags,16
   1534c:	60e2                	ld	ra,24(sp)
   1534e:	6442                	ld	s0,16(sp)
   15350:	64a2                	ld	s1,8(sp)
   15352:	6105                	addi	sp,sp,32
   15354:	8082                	ret
   15356:	ff15071b          	addiw	a4,a0,-15
   1535a:	00e41733          	sll	a4,s0,a4
   1535e:	4401                	li	s0,0
   15360:	b761                	j	152e8 <__extenddftf2+0x7c>

0000000000015362 <__clzdi2>:
   15362:	03800793          	li	a5,56
   15366:	00f55733          	srl	a4,a0,a5
   1536a:	0ff77693          	zext.b	a3,a4
   1536e:	ee99                	bnez	a3,1538c <__clzdi2+0x2a>
   15370:	17e1                	addi	a5,a5,-8
   15372:	fbf5                	bnez	a5,15366 <__clzdi2+0x4>
   15374:	00003717          	auipc	a4,0x3
   15378:	d8c73703          	ld	a4,-628(a4) # 18100 <_GLOBAL_OFFSET_TABLE_+0x8>
   1537c:	953a                	add	a0,a0,a4
   1537e:	00054503          	lbu	a0,0(a0)
   15382:	04000793          	li	a5,64
   15386:	40a7853b          	subw	a0,a5,a0
   1538a:	8082                	ret
   1538c:	04000513          	li	a0,64
   15390:	40f507b3          	sub	a5,a0,a5
   15394:	853a                	mv	a0,a4
   15396:	00003717          	auipc	a4,0x3
   1539a:	d6a73703          	ld	a4,-662(a4) # 18100 <_GLOBAL_OFFSET_TABLE_+0x8>
   1539e:	953a                	add	a0,a0,a4
   153a0:	00054503          	lbu	a0,0(a0)
   153a4:	40a7853b          	subw	a0,a5,a0
   153a8:	8082                	ret
