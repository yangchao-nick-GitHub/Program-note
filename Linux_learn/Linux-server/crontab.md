权限设置：
/etc/cron.deny		设置不允许执行cron任务的用户

配置文件：
1./var/spool/cron/crontabs/root 		root使用crontab -e设置的定时任务
2./etc/crontab			不区分用户，全局执行的系统任务 修改了需要重启cron服务
3./etc/cron.d/*   

操作定时任务
编辑任务crontab -e
删除任务crontab -r
查看任务crontab -l



