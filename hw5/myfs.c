#include <asm/atomic.h>
#include <linux/fs.h> /* This is where libfs stuff is declared */
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/pagemap.h> /* PAGE_CACHE_SIZE */
#include <linux/string.h>
#include <linux/uaccess.h> /* copy_to_user */

MODULE_LICENSE("GPL");

#define MYFS_MAGIC 0x20210607

static struct dataAb {
    atomic_t *a;
    atomic_t *b;
} abData;

static struct inode *myfs_make_inode(struct super_block *sb, int mode) {
    struct inode *ret = new_inode(sb);

    if (ret) {
        ret->i_mode = mode;
        ret->i_uid.val = 0;
        ret->i_gid.val = 0;
        ret->i_blocks = 0;
        ret->i_atime = current_kernel_time();
        ret->i_mtime = current_kernel_time();
        ret->i_ctime = current_kernel_time();
    }
    return ret;
}

static int myfs_open(struct inode *inode, struct file *filp) {
    filp->private_data = inode->i_private;
    return 0;
}

#define TMPSIZE 20

static ssize_t myfs_read_file(struct file *filp, char *buf, size_t count, loff_t *offset) {
    atomic_t *counter = (atomic_t *)filp->private_data;
    //int *counter;
    struct dataAb *dataStog;
    struct dentry *dentry = filp->f_path.dentry;
    const char *name = dentry->d_name.name;
    const char *strA = "a";
    const char *strB = "b";
    const char *strAdd = "add";
    const char *strSub = "sub";

    int v, len;
    char tmp[TMPSIZE];
    
    if (*offset > 0) {
        printk("*offset > 0");
    } else {
        printk("--%s--", name);
        if (strcmp(name, strA) == 0) {
            v = atomic_read(counter);
        printk("a = %d", v);
        } else if (strcmp(name, strB) == 0) {
            v = atomic_read(counter);
        printk("b = %d", v);
        } else if (strcmp(name, strAdd) == 0) {
            dataStog = (struct dataAb *)filp->private_data;
            atomic_add(atomic_read(dataStog->b), dataStog->a); // a+=b
            v = atomic_read(dataStog->a);
            atomic_sub(atomic_read(dataStog->b), dataStog->a); // a-=b => 為了不要動到a的數值
            printk("a+b = %d", v);
        } else if (strcmp(name, strSub) == 0) {
            dataStog = (struct dataAb *)filp->private_data;
            atomic_sub(atomic_read(dataStog->b), dataStog->a);
            v = atomic_read(dataStog->a);
            atomic_add(atomic_read(dataStog->b), dataStog->a);
            printk("a-b = %d", v);
        }
    }

    len = snprintf(tmp, TMPSIZE, "%d\n", v);
    if (*offset > len)
        return 0;
    if (count > len - *offset)
        count = len - *offset;

    if (copy_to_user(buf, tmp + *offset, count))
        return -EFAULT;
    *offset += count;
    return count;
}

static ssize_t myfs_write_file(struct file *filp, const char *buf, size_t count, loff_t *offset) {
    atomic_t *counter = (atomic_t *)filp->private_data;

    char tmp[TMPSIZE];

    if (*offset != 0)
        return -EINVAL;

    if (count >= TMPSIZE)
        return -EINVAL;
    memset(tmp, 0, TMPSIZE);
    if (copy_from_user(tmp, buf, count))
        return -EFAULT;

    atomic_set(counter, simple_strtol(tmp, NULL, 10));
    return count;
}

static struct file_operations myfs_file_ops = {
    .open = myfs_open,
    .read = myfs_read_file,
    .write = myfs_write_file,
};

static struct dentry *myfs_create_file(struct super_block *sb, struct dentry *dir, const char *name, atomic_t *counter) {
    struct dentry *dentry;
    struct inode *inode;
    struct qstr qname;

    qname.name = name;
    qname.len = strlen(name);
    qname.hash = full_name_hash(name, qname.len);

    dentry = d_alloc(dir, &qname);
    if (!dentry)
        goto out;
    inode = myfs_make_inode(sb, S_IFREG | 0644);
    if (!inode)
        goto out_dput;
    inode->i_fop = &myfs_file_ops;
    inode->i_private = counter;

    d_add(dentry, inode);
    return dentry;

    out_dput:
        dput(dentry);
    out:
        return 0;
}

static struct dentry *myfs_create_func_file(struct super_block *sb, struct dentry *dir, const char *name, struct dataAb *counter) {
    struct dentry *dentry;
    struct inode *inode;
    struct qstr qname;

    qname.name = name;
    qname.len = strlen(name);
    qname.hash = full_name_hash(name, qname.len);

    dentry = d_alloc(dir, &qname);
    if (!dentry)
        goto out;
    inode = myfs_make_inode(sb, S_IFREG | 0644);
    if (!inode)
        goto out_dput;
    inode->i_fop = &myfs_file_ops;
    inode->i_private = counter;

    d_add(dentry, inode);
    return dentry;

    out_dput:
        dput(dentry);
    out:
        return 0;
}

static struct dentry *myfs_create_dir(struct super_block *sb, struct dentry *parent, const char *name) {
    struct dentry *dentry;
    struct inode *inode;
    struct qstr qname;

    qname.name = name;
    qname.len = strlen(name);
    qname.hash = full_name_hash(name, qname.len);
    dentry = d_alloc(parent, &qname);
    if (!dentry)
        goto out;

    inode = myfs_make_inode(sb, S_IFDIR | 0644);
    if (!inode)
        goto out_dput;
    inode->i_op = &simple_dir_inode_operations;
    inode->i_fop = &simple_dir_operations;

    d_add(dentry, inode);
    return dentry;

    out_dput:
        dput(dentry);
    out:
        return 0;
}

static atomic_t a, b;

static void myfs_create_files(struct super_block *sb, struct dentry *root) {
    struct dentry *inputdir;
    struct dentry *outputdir;

    inputdir = myfs_create_dir(sb, root, "input");
    outputdir = myfs_create_dir(sb, root, "output");

    atomic_set(&a, 0);
    atomic_set(&b, 0);

    if (inputdir) {
        myfs_create_file(sb, inputdir, "a", &a);
        myfs_create_file(sb, inputdir, "b", &b);
    }

    abData.a = &a;
    abData.b = &b;
    printk("creating files");
    if (outputdir) {
        myfs_create_func_file(sb, outputdir, "add", &abData);
        myfs_create_func_file(sb, outputdir, "sub", &abData);
    }
}

static struct super_operations myfs_s_ops = {
    .statfs = simple_statfs,
    .drop_inode = generic_delete_inode,
};

static int myfs_fill_super(struct super_block *sb, void *data, int silent) {
    struct inode *root;
    struct dentry *root_dentry;

    sb->s_blocksize = PAGE_SIZE;
    sb->s_blocksize_bits = PAGE_SHIFT;
    sb->s_magic = MYFS_MAGIC;
    sb->s_op = &myfs_s_ops;

    root = myfs_make_inode(sb, S_IFDIR | 0755);
    if (!root)
        goto out;
    root->i_op = &simple_dir_inode_operations;
    root->i_fop = &simple_dir_operations;

    root_dentry = d_make_root(root);
    if (!root_dentry)
        goto out_iput;
    sb->s_root = root_dentry;

    myfs_create_files(sb, root_dentry);
    return 0;

    out_iput:
        iput(root);
    out:
        return -ENOMEM;
}

static struct dentry *myfs_get_super(struct file_system_type *fst, int flags, const char *devname, void *data) { 
    return mount_nodev(fst, flags, data, myfs_fill_super);
}

static struct file_system_type myfs_type = {
    .owner = THIS_MODULE,
    .name = "myfs",
    .mount = myfs_get_super,
    .kill_sb = kill_litter_super,
};

static int __init myfs_init(void) { 
    return register_filesystem(&myfs_type); 
}

static void __exit myfs_exit(void) { 
    unregister_filesystem(&myfs_type); 
}

module_init(myfs_init);
module_exit(myfs_exit);