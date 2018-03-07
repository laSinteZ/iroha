# How to use new ansible playbook

## Main ideas
This playbook allows you to deploy multiple iroha peers on one node. For example, you have 4 hosts that are in the same network. You want run 16 iroha peers on each host. Actually, you can run as many iroha peers on one host as you want (but no more that 15000). 

It works in the following way:
- docker-compose.yml file is generated for each host that contains as many nodes as you would like (this value is set by variable `nodes_in_region` in `playbooks/iroha/group_vars/<group_name>.yml`)
- all configs are stored on each host's `/opt/docker/iroha/conf$KEY` folder. There are:
    - genesis.block
    - config.sample
    - node$KEY.priv
    - node$KEY.pub
- it starts using `docker-compose` command

Let's discuss how it works in details.

## Inventory (hosts.list)
```
[iroha-east]
iroha-bench1 ansible_host=45.76.152.117 ansible_user=root key=0

[iroha-west]
iroha-bench2 ansible_host=198.13.61.246 ansible_user=root key=16

[iroha-south]
iroha-bench3 ansible_host=104.207.152.219 ansible_user=root key=32

[iroha-north]
iroha-bench4 ansible_host=45.76.212.185 ansible_user=root key=48

[locals]
localhost           ansible_connection=local
```

As you can see, basic host field in group contains `hostname`, `ansible_host <ip>`, `ansible_user`, and `key` field. 

`key` is a node ID in a iroha network. In this particular playbook this value means the start of the count. Values `key` and `nodes_in_region` are used in the following manner:
- for host iroha-bench1 we have 16 iroha peers. First peer ID will be key=0, for second key=1,...,up to key=15
- for host iroha-bench2 we have another 16 iroha peers. Their IDs will start from 16 to 31.

> `nodes_in_region` variable could be set at `playbooks/iroha/group_vars/<group_name>.yml`

E.g. one would like to have 13 peers on `iroha-bench2`: `key`=16 is the initial value, `nodes_in_region` must be set to 13. Therefore on `iroha-bench3` --> `key`=29.

## Peer configs

There is no need to describe them as they are generated automaitally. Port management is also automated. If you want to see how it works --> you can see `roles/iroha-node/tasks/ubuntu.yml` and templates `roles/iroha-node/templates/`

## How to launch

After `hosts.list` inventory file is configured one could launch the playbook.

```
ansible-playbook -i inventory/hosts.list playbooks/iroha/iroha-bench.yml --private-key=~/.ssh/<key>
```
, where you should specify your SSH key.


## Requirements
1) **iroha-cli** must be installed and could be accessed using PATH variable (e.g. /usr/bin). This is due to keys and genesis.block is generated on your local host and stored on /tmp/iroha-bench
