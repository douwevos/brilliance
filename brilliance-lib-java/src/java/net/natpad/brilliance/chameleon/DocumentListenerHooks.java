package net.natpad.brilliance.chameleon;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.atomic.AtomicInteger;

import net.natpad.brilliance.Logger;

public class DocumentListenerHooks implements Logger {

	private final Map<Long, Hook> hooks = new ConcurrentHashMap<>();
	
	private static final DocumentListenerHooks instance = new DocumentListenerHooks();
	
	public static DocumentListenerHooks instance() {
		return instance;
	}

	public Hook refHook(long nativeId) {
		Hook hook = hooks.computeIfAbsent(nativeId, (k) -> { return new Hook(k);});
		hook.ref();
		return hook;
	}
	
	public void unrefHook(Hook hook) {
		if (hook.unref()) {
			hooks.remove(hook.nativeId);
		}
	}
	
	public static class Hook implements Logger {
		public final long nativeId;
		private final AtomicInteger refCount = new AtomicInteger();
		
		private final List<IDocumentListener> listeners = new ArrayList<>();
		
		public Hook(long nativeId) {
			this.nativeId = nativeId;
			log("registering Hook");
			Chameleon.documentHookListener(nativeId, this);
			log("registering Hook done");
		}
		
		public void ref() {
			refCount.incrementAndGet();
		}
		
		public boolean unref() {
			return refCount.decrementAndGet() <= 0;
		}
		
		@Override
		public boolean equals(Object obj) {
			if (obj instanceof Hook) {
				return nativeId == ((Hook) obj).nativeId;
			}
			return false;
		}
		
		@Override
		public int hashCode() {
			return (int) nativeId;
		}

		public synchronized void add(IDocumentListener listener) {
			listeners.add(listener);
		}
		
		
		public void onNewRevision(long revisionId) {
			Revision revision = revisionId==0 ? null : new Revision(revisionId);
			for(IDocumentListener listener : listeners) {
				listener.onNewRevision(revision);
			}
		}
	}
	
}
