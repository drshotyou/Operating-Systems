#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <inttypes.h>
#include "os_hw4_driver.h"

int fd;

// Obtain my cr3 value (a.k.a. PML4 table physical address)
uint64_t get_cr3_value()
{
	struct ioctl_arg cmd;
	int ret;
	cmd.request[0] = IO_CR3;
	ret = ioctl(fd, IOCTL_REQUEST, &cmd);
	return cmd.ret;
}

// Given a physical address, return the value
uint64_t read_physical_address(uint64_t physical_address)
{
	struct ioctl_arg cmd;
	int ret;
	cmd.request[0] = IO_READ;
	cmd.request[1] = physical_address;
	ret = ioctl(fd, IOCTL_REQUEST, &cmd);
	return cmd.ret;
}

// Write value to a physical address
void write_physical_address(uint64_t physical_address, uint64_t value)
{
	struct ioctl_arg cmd;
	int ret;
	cmd.request[0] = IO_WRITE;
	cmd.request[1] = physical_address;
	cmd.request[2] = value;
	ret = ioctl(fd, IOCTL_REQUEST, &cmd);
}

size_t get_pml4(size_t virtual_addr) {
	size_t pml4 = ((size_t)virtual_addr)>>39;
	//printf("pml4 = %lx\n", pml4);
	pml4 = pml4 * ((size_t)8);
	//printf("pml4*8= %lx\n", pml4);
	return pml4;
}

size_t get_pdpt(size_t virtual_addr) {
	size_t pdpt = ((size_t)virtual_addr)>>30;
	pdpt = (0x1FF & pdpt);
	//printf("pdpt = %lx\n", pdpt);
	pdpt = pdpt * ((size_t)8);
	//printf("pdpt*8 = %lx\n", pdpt);
	return pdpt;
}

size_t get_pd(size_t virtual_addr) {
	size_t pd = ((size_t)virtual_addr)>>21;
	pd = (0x1FF & pd);
	//printf("pd = %lx\n", pd);
	pd = pd * ((size_t)8);
	//printf("pd*8 = %lx\n", pd);
	return pd;
}

size_t get_pt(size_t virtual_addr) {
	size_t pt = ((size_t)virtual_addr)>>12;
	pt = (0x1FF & pt);
	//printf("pd = %lx\n", pt);
	pt = pt * ((size_t)8);
	//printf("pd*8 = %lx\n", pt);
	return pt;
}

size_t page_table_physical_page(char* ptr, uint64_t cr3_val) {
	size_t pml4 = get_pml4((size_t)ptr);
	size_t pml4_entry = (((size_t)cr3_val)>>12)<<12;
	pml4_entry = (pml4_entry + pml4);
	uint64_t pml4_content = read_physical_address(pml4_entry);
	//printf("content of pml4 entry:%lx\n", pml4_content);


	size_t pdpt = get_pdpt((size_t)ptr);
	size_t pdpt_entry = (((size_t)pml4_content)>>12)<<12;
	pdpt_entry = (pdpt_entry & 0x0000000FFFFFF000);
	//printf("%lx\n", pdpt_entry);
	pdpt_entry = (pdpt_entry + pdpt);
	//printf("%lx\n", pdpt_entry);
	uint64_t pdpt_content = read_physical_address(pdpt_entry);
	//printf("content of pdpt entry:%lx\n", pdpt_content);


	size_t pd = get_pd((size_t)ptr);
	size_t pd_entry = (((size_t)pdpt_content)>>12)<<12;
	pd_entry = (pd_entry & 0x0000000FFFFFF000);
	//printf("%lx\n", pd_entry);
	pd_entry = (pd_entry + pd);
	//printf("%lx\n", pd_entry);
	uint64_t pd_content = read_physical_address(pd_entry);
	//printf("content of pd entry:%lx\n", pd_content);


	size_t pt = get_pt((size_t)ptr);
	size_t pt_entry = (((size_t)pd_content)>>12)<<12;
	pt_entry = (pt_entry & 0x0000000FFFFFF000);
	//printf("%lx\n", pt_entry);
	pt_entry = (pt_entry + pt);
	//printf("%lx\n", pt_entry);
	uint64_t pt_content = read_physical_address(pt_entry);
	//printf("content of pt entry:%lx\n", pt_content);

	return pt_entry;
	
	size_t p_entry = (((size_t)pt_content)>>12)<<12;
	p_entry = (p_entry & 0x0000000FFFFFF000);
	printf("%lx\n", p_entry);
	p_entry = (p_entry + 0x123);
	printf("%lx\n", p_entry);
	uint64_t content = read_physical_address(p_entry);
	printf("content of x entry:%lx\n", content);

	return p_entry;
}

int main()
{
	char *x = (char*)aligned_alloc(4096, 4096) + 0x123;
	char *y = (char*)aligned_alloc(4096, 4096) + 0x123;
	strcpy(x, "This is OS homework 4.");
	strcpy(y, "You have to modify my page table.");

	fd = open("/dev/os", O_RDONLY);
	if(fd < 0) 
	{
		printf("Cannot open device!\n");
		return 0;
	}

	printf("Before\n");
	printf("x : %s\n", x);
	printf("y : %s\n", y);

	uint64_t cr3_val = get_cr3_value();
	//printf("CR3 address:[%" PRIu64 "]\n", cr3_val);

	char* x_parser = x;

	size_t pml4 = get_pml4((size_t)x_parser);
	size_t pml4_entry = (((size_t)cr3_val)>>12)<<12;
	pml4_entry = (pml4_entry + pml4);
	uint64_t pml4_content = read_physical_address(pml4_entry);
	//printf("content of pml4 entry:%lx\n", pml4_content);

	size_t pdpt = get_pdpt((size_t)x_parser);
	size_t pdpt_entry = (((size_t)pml4_content)>>12)<<12;
	pdpt_entry = (pdpt_entry & 0x0000000FFFFFF000);
	pdpt_entry = (pdpt_entry + pdpt);
	uint64_t pdpt_content = read_physical_address(pdpt_entry);
	//printf("content of pdpt entry:%lx\n", pdpt_content);


	size_t pd = get_pd((size_t)x_parser);
	size_t pd_entry = (((size_t)pdpt_content)>>12)<<12;
	pd_entry = (pd_entry & 0x0000000FFFFFF000);
	pd_entry = (pd_entry + pd);
	uint64_t pd_content = read_physical_address(pd_entry);
	//printf("content of pd entry:%lx\n", pd_content);

	size_t pt = get_pt((size_t)x_parser);
	size_t pt_entry = (((size_t)pd_content)>>12)<<12;
	pt_entry = (pt_entry & 0x0000000FFFFFF000);
	pt_entry = (pt_entry + pt);
	uint64_t pt_content = read_physical_address(pt_entry);
	//printf("content of pt entry:%lx\n", pt_content);

	//size_t p_entry = (((size_t)pt_content)>>12)<<12;
	//p_entry = (p_entry & 0x0000000FFFFFF000);
	//p_entry = (p_entry + 0x123);
	//uint64_t x_content = read_physical_address(p_entry);
	//printf("content of x entry:%lx\n", x_content);

	char* y_parser = y;
	size_t y_entry = page_table_physical_page(y_parser, cr3_val);
	size_t y_content = read_physical_address(y_entry);
	write_physical_address(y_entry, pt_content);

	/* TODO 1 */
	// ------------------------------------------------
	// Modify page table entry of y
	// Let y point to x's physical address
	// ------------------------------------------------
	getchar();

	printf("After modifying page table\n");
	printf("x : %s\n", x);
	printf("y : %s\n", y);

	getchar();

	strcpy(y, "When you modify y, x is modified actually.");
	printf("After modifying string y\n");
	printf("x : %s\n", x);
	printf("y : %s\n", y);

	write_physical_address(y_entry, y_content);
	/* TODO 2 */
	// ------------------------------------------------
	// Recover page table entry of yx_content
	// Let y point to its original address
	// You may need to store y's original address at previous step
	// ------------------------------------------------

	getchar();
	printf("After recovering page table of y\n");
	printf("x : %s\n", x);
	printf("y : %s\n", y);

	close(fd);
}
