del *.bak /s
del *.ddk /s
del *.edk /s
del *.lst /s
del *.lnp /s
del *.mpf /s
del *.mpj /s
del *.obj /s
del *.omf /s
::del *.opt /s  ::������ɾ��JLINK������
del *.plg /s
del *.rpt /s
del *.tmp /s
del *.__i /s
del *.crf /s
del *.o /s
del *.d /s
del *.axf /s
del *.tra /s
del *.dep /s           
del JLinkLog.txt /s

del *.iex /s
del *.htm /s
del *.sct /s
del *.map /s
del *.Administrator
cls 
    @ECHO OFF 

����:CONFIRM 
����echo �Ƿ�����������(Y/N) 

����set/p "cho=>"

����if %cho%==Y goto Rename 

����if %cho%==y goto Rename 

����if %cho%==n goto END 

����if %cho%==N goto END 

����:Rename

    echo �������¹��̵����֣� 

����set/p "name=>" 

����ren .\USER\*.uvproj %name%.uvproj 

    ren .\USER\*.uvopt  %name%.uvopt
   


����:End ���� 
