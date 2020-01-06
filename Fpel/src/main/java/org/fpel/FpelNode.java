package org.fpel;

import java.util.Arrays;
import java.util.List;
import java.util.Objects;
import java.util.concurrent.CopyOnWriteArrayList;
import java.util.stream.Collectors;
import java.util.stream.Stream;

public abstract class FpelNode {

	private final int left;
	private final int right;
	
	private final List<FpelNode> children;
	private FpelNode parentNode;
	
	public FpelNode(FpelNode ... children) {
		if (children != null) {
			List<FpelNode> collected = Arrays.asList(children).stream().filter(Objects::nonNull).collect(Collectors.toList());
			this.children = new CopyOnWriteArrayList<FpelNode>(collected);
			this.children.stream().forEach(p -> p.parentNode = this);
			this.left = this.children.get(0).left;
			this.right = this.children.get(this.children.size()-1).right;
		} else {
			this.children = null;
			this.left = -1;
			this.right = -1;
		}
	}
	
	public FpelNode(int left, int right) {
		this.left = left;
		this.right = right;
		children = null;
	}

	public int getLeft() {
		return left;
	}

	public int getRight() {
		return right;
	}
	
	public Stream<FpelNode> streamChildren() {
		return children==null ? Stream.empty() : children.stream();
	}

	public FpelNode getParentNode() {
		return parentNode;
	}
	
	
	
	@Override
	public String toString() {
		return this.getClass().getSimpleName()+"[left="+left+", right="+right+"]";
	}

}
